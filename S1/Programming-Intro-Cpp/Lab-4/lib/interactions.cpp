#include <hamming_archive.h>
#include <interactions.h>

#include <filesystem>
#include <iostream>

void interactions::create(const Config& config) {
    if (config.args.empty()) {
        resolve_error("No files to create archive file from were specified...");
        return;
    }

    fs::path archive_path = config.path;

    if (is_directory(archive_path)) {
        archive_path /= fs::path(config.args[0]).stem();
    }

    if (!archive_path.has_extension() || archive_path.extension() != ".haf") {
        archive_path.replace_extension(".haf");
    }

    HammingArchive archive(archive_path, config.word_size);

    archive.append(std::vector<fs::path>(config.args.begin(), config.args.end()));

    archive.write();
}

void interactions::extract(const Config& config) {
    if (!HammingArchive::is_archive(config.path)) {
        resolve_error("No valid archive file was specified.");
        return;
    }

    const fs::path dest = config.path.stem();

    create_directories(dest);

    const HammingArchive archive(config.path, config.word_size);

    if (config.args.empty()) {
        archive.extract(std::vector{std::string("*")}, dest);
        return;
    }

    archive.extract(config.args, dest);
}

void interactions::append(const Config& config) {
    if (!HammingArchive::is_archive(config.path)) {
        resolve_error("No valid archive file was specified.");
        return;
    }

    HammingArchive archive(config.path, config.word_size);

    if (config.args.empty()) {
        resolve_error("No files to create archive from were specified.");
        return;
    }

    archive.append(std::vector<fs::path>(config.args.begin(), config.args.end()));

    archive.write();
}

void interactions::remove(const Config& config) {
    if (!HammingArchive::is_archive(config.path)) {
        resolve_error("No valid archive file was specified.");
        return;
    }

    HammingArchive archive(config.path, config.word_size);

    if (config.args.empty()) {
        resolve_error("No files to create archive from were specified.");
        return;
    }

    archive.remove(config.args);

    archive.write();
}

void interactions::concat(const Config& config) {
    if (config.args.empty()) {
        resolve_error("Cannot concatenate nothing...");
        return;
    }

    fs::path archive_path = config.path;

    if (is_directory(archive_path)) {
        archive_path /= fs::path("merged_" + fs::path(config.args[0]).filename().string());
    }

    HammingArchive archive(archive_path, config.word_size);

    std::vector<HammingArchive> archives;

    std::transform(config.args.cbegin(), config.args.cend(), std::back_inserter(archives), [&config](const auto& item)
    {
        return HammingArchive(item, config.word_size);
    });

    archive.concat(archives);

    archive.write();
}

void interactions::list(const Config& config) {
    if (!HammingArchive::is_archive(config.path)) {
        resolve_error("No valid archive file was specified.");
        return;
    }

    const HammingArchive archive(config.path, config.word_size);

    if (config.args.empty()) {
        for (const auto& [name, size, actual] : archive.explore("*")) {
            std::cout << " - " << name << " :of: " << actual << " Bytes." << '\n';
        }

        std::cout.flush();

        return;
    }

    for (const auto& [name, size, actual] : archive.explore(config.args.front())) {
        std::cout << " - " << name << " :of: " << actual << " Bytes." << '\n';
    }

    std::cout.flush();
}

void interactions::resolve_error(const std::string& message) {
    std::cout << message << '\n';
    std::cout << "Use -h (--help) flag to learn about this program." << std::endl;
}
