#pragma once

#include <ios>
#include <vector>
#include <cstddef>
#include <fstream>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

namespace encoder {
    using Bits = std::vector<bool>;

    [[nodiscard]] Bits to_bits(const char* bytes, size_t size);
    void to_bytes(const Bits& bits, char* out);

    void generate(Bits& data);

    void recover(Bits& data);

    void encode(Bits& data);

    void decode(Bits& data);

    class IEStream {
        std::streamsize word_size_;
        std::streamsize code_size_;
        std::ifstream stream_;
        bool state_;
    public:
        IEStream(const fs::path& file, std::streamsize word_size);

        IEStream& read(char* dest);

        IEStream& read(char* dest, std::streamsize size);

        IEStream& ignore(std::streamsize size, bool raw = false);

        IEStream& close();

        explicit operator bool() const;
        bool operator!() const;
    };

    class OEStream {
        std::streamsize word_size_;
        std::streamsize code_size_;
        std::ofstream stream_;
    public:
        OEStream(const fs::path& file, std::streamsize word_size);

        OEStream& write(const char* src);

        OEStream& write(const char* src, std::streamsize size);

        OEStream& close();
    };
}
