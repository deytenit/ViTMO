#pragma once

#include <chrono>
#include <filesystem>
#include <ios>
#include <iterator>
#include <random>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class HammingArchive {
public:
    struct FileMetadata {
        char name[256] = {0};
        uint64_t size = 0;
        uint64_t actual = 0;
    };

    static constexpr char kMagic[6] = {'H', 'A', 'M', 'A', 'R', 'C'};
    static constexpr std::streamsize kBlockMaxSize = 512;
    static constexpr uint32_t kHashSize = 32;

private:
    fs::path path_;
    fs::path tmp_dir_;
    fs::path tmp_path_;
    std::streamsize word_size_;
    std::streamsize block_size_;

public:
    HammingArchive(fs::path path, std::streamsize word_size);

    [[nodiscard]] std::vector<FileMetadata> explore(const std::string& wildcard) const;

    void extract(const std::vector<std::string>& wildcards, fs::path dest) const;

    void append(const std::vector<fs::path>& paths);

    void remove(const std::vector<std::string>& wildcards);

    void clear() const;

    void write() const;

    void concat(const std::vector<HammingArchive>& archives);

    static bool is_archive(const fs::path& path);

private:
    void deconflict(const std::vector<fs::path>& paths);

    static bool match_wildcard(const std::string& str, const std::string& wildcard);

    static std::string hash(uint32_t);
};
