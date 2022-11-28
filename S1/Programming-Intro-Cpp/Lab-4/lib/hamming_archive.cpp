#include <filesystem>
#include <hamming_archive.h>

#include <iostream>
#include <regex>

#include "encoder.h"

HammingArchive::HammingArchive(fs::path path, const std::streamsize word_size)
    : path_(std::move(path)),
      tmp_dir_(fs::temp_directory_path() / fs::path("HamArc")),
      tmp_path_(tmp_dir_ / fs::path(hash(kHashSize) + path_.extension().string())),
      word_size_(word_size),
      block_size_(word_size_ * (kBlockMaxSize / word_size_)) {
    create_directories(tmp_dir_);

    if (is_archive(path_)) {
        copy_file(path_, tmp_path_, fs::copy_options::overwrite_existing);
    }
    else {
        clear();
    }
}

std::vector<HammingArchive::FileMetadata> HammingArchive::explore(const std::string& wildcard) const {
    std::vector<FileMetadata> result;

    const auto buffer = new char[block_size_];
    std::memset(buffer, 0, block_size_);

    encoder::IEStream ies(tmp_path_, word_size_);

    ies.ignore(sizeof kMagic, true);

    while (true) {
        if (!ies.read(buffer, block_size_)) {
            break;
        }

        const FileMetadata* meta = reinterpret_cast<FileMetadata*>(buffer);

        if (match_wildcard(meta->name, wildcard)) {
            result.push_back(*meta);
        }

        if (!ies.ignore(static_cast<std::streamsize>(meta->size))) {
            break;
        }
    }

    ies.close();
    delete[] buffer;

    return result;
}

void HammingArchive::extract(const std::vector<std::string>& wildcards, fs::path dest) const {
    if (!is_directory(dest)) {
        dest = path_.stem();
        create_directory(dest);
    }

    auto buffer = new char[block_size_];
    std::memset(buffer, 0, block_size_);

    encoder::IEStream ies(tmp_path_, word_size_);

    ies.ignore(sizeof kMagic, true);

    while (true) {
        if (!ies.read(buffer, block_size_)) {
            break;
        }

        const auto [name, size, actual](*reinterpret_cast<FileMetadata*>(buffer));

        bool pass = true;

        for (const auto& wildcard : wildcards) {
            if (match_wildcard(name, wildcard)) {
                pass = false;
                break;
            }
        }

        if (pass) {
            ies.ignore(static_cast<std::streamsize>(size));
            continue;
        }

        fs::path out_path = dest / fs::path(name);

        std::ofstream out(out_path, std::ios::binary | std::ios::out);

        for (size_t i = 0; i < size / block_size_ - 1; ++i) {
            std::memset(buffer, 0, block_size_);
            ies.read(buffer, block_size_);

            out.write(buffer, block_size_);
        }

        std::memset(buffer, 0, block_size_);
        ies.read(buffer, block_size_);
        out.write(buffer, static_cast<std::streamsize>(actual) % block_size_);

        out.close();
    }

    ies.close();
    delete[] buffer;
}

void HammingArchive::append(const std::vector<fs::path>& paths) {
    deconflict(paths);

    encoder::OEStream oes(tmp_path_, word_size_);

    for (const auto& file : paths) {
        if (!is_regular_file(file)) {
            continue;
        }

        auto buffer = new char[block_size_];
        std::memset(buffer, 0, block_size_);

        std::ifstream in(file, std::ios::binary | std::ios::in);

        FileMetadata meta;

        std::string name = file.filename().string();
        std::copy(name.begin(), name.end(), meta.name);

        meta.size = block_size_ * (file_size(file) / block_size_ + 1);
        meta.actual = file_size(file);

        std::memcpy(buffer, &meta.name, sizeof meta.name);
        std::memcpy(buffer + sizeof meta.name, &meta.size, sizeof meta.size);
        std::memcpy(buffer + sizeof meta.name + sizeof meta.size, &meta.actual, sizeof meta.actual);

        oes.write(buffer, block_size_);

        for (size_t i = 0; i < meta.size / block_size_; ++i) {
            in.read(buffer, block_size_);

            oes.write(buffer, block_size_);
        }

        in.close();
        delete[] buffer;
    }

    oes.close();
}

void HammingArchive::remove(const std::vector<std::string>& wildcards) {
    const fs::path dest = tmp_dir_ / fs::path(hash(kHashSize));

    create_directories(dest);

    extract(std::vector<std::string>{"*"}, dest);

    for (const auto& entry : fs::directory_iterator{dest}) {
        for (const auto& wildcard : wildcards) {
            if (match_wildcard(entry.path().filename().string(), wildcard)) {
                fs::remove(entry);
            }
        }
    }

    clear();

    const fs::directory_iterator begin{dest}, end;

    append(std::vector<fs::path>(begin, end));

    remove_all(dest);
}

void HammingArchive::clear() const {
    std::ofstream out(tmp_path_, std::ios::out | std::ios::trunc);
    out.write(kMagic, sizeof kMagic);
    out.close();
}

void HammingArchive::write() const {
    copy_file(tmp_path_, path_, fs::copy_options::overwrite_existing);
}

void HammingArchive::concat(const std::vector<HammingArchive>& archives) {
    for (const auto& archive : archives) {
        fs::path dest = tmp_dir_ / fs::path(hash(kHashSize));

        create_directory(dest);

        archive.extract(std::vector<std::string>{"*"}, dest);

        const fs::directory_iterator begin{dest}, end;

        append(std::vector<fs::path>(begin, end));

        remove_all(dest);
    }
}

bool HammingArchive::is_archive(const fs::path& path) {
    std::ifstream in(path, std::ios::binary | std::ios::in);

    char magic[sizeof kMagic];

    if (!in) {
        return false;
    }

    in.read(magic, sizeof magic);

    in.close();

    return std::equal(std::begin(magic), std::end(magic), std::begin(kMagic));
}

void HammingArchive::deconflict(const std::vector<fs::path>& paths) {
    std::vector<std::string> out;
    std::vector<FileMetadata> metas = explore("*");

    for (const auto& path : paths) {
        if (std::find_if(metas.cbegin(), metas.cend(), [&path](const auto& meta)
        {
            return path.filename().compare(meta.name) == 0;
        }) != metas.end()) {
            out.push_back(path.filename().string());
        }
    }

    if (!out.empty()) {
        remove(out);
    }
}

bool HammingArchive::match_wildcard(const std::string& str, const std::string& wildcard) {
    const std::pair<char, std::string> any_more = {'*', ".*"};
    const std::pair<char, std::string> any_single = {'?', "."};

    std::string pattern;

    for (const char i : wildcard) {
        if (i == any_more.first) {
            pattern += any_more.second;
        }
        else if (i == any_single.first) {
            pattern += any_single.second;
        }
        else {
            pattern += i;
        }
    }

    const std::regex regex(pattern);

    return std::regex_match(str, regex);
}

std::string HammingArchive::hash(const uint32_t size) {
    constexpr char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string tmp_s;
    tmp_s.reserve(size);

    std::random_device seed;
    std::mt19937_64 rnd(seed());

    std::uniform_int_distribution<size_t> distribution(0, std::strlen(charset));

    for (size_t i = 0; i < size; ++i) {
        tmp_s += charset[distribution(rnd)];
    }

    return tmp_s;
}
