#include <encoder.h>

#include <iostream>

encoder::Bits encoder::to_bits(const char* bytes, const size_t size) {
    Bits result(size * 8);

    for (size_t i = 0; i < size; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            result[8 * i + j] = bytes[i] & 1 << (7 - j);
        }
    }

    return result;
}

void encoder::to_bytes(const Bits& bits, char* out) {
    for (Bits::size_type i = 0; i < bits.size(); ++i) {
        if (bits[i]) {
            out[i / 8] |= 1 << (7 - i) % 8;
        }
    }
}

void encoder::generate(Bits& data) {
    for (size_t i = 1; i <= data.size(); i <<= 1) {
        for (size_t j = i; j <= data.size(); j += i << 1) {
            for (size_t k = j; k < std::min(j + i, data.size() + 1); ++k) {
                data[i - 1] = data[i - 1] != data[k - 1];
            }
        }
    }
}

void encoder::recover(Bits& data) {
    Bits temp = data;

    generate(temp);

    bool is_broken = false;
    size_t broken_bit = 0;

    for (size_t i = 0; i < temp.size(); ++i) {
        if (data[i] != temp[i]) {
            is_broken = true;
            broken_bit += i + 1;
        }
    }

    if (is_broken) {
        data[broken_bit - 1] = !data[broken_bit - 1];
    }
}

void encoder::encode(Bits& data) {
    Bits result;

    size_t power = 1;
    size_t i = 0;
    auto it = data.begin();

    while (it != data.end()) {
        if (i == power - 1) {
            result.push_back(false);
            power <<= 1;
        }
        else {
            result.push_back(*it++);
        }
        ++i;
    }

    generate(result);

    data = result;
}

void encoder::decode(Bits& data) {
    Bits result;

    size_t power = 1;

    for (size_t i = 0; i < data.size(); ++i) {
        if (i == power - 1) {
            power <<= 1;
        }
        else {
            result.push_back(data[i]);
        }
    }

    data = result;
}

encoder::IEStream::IEStream(const fs::path& file, const std::streamsize word_size)
    : stream_(file, std::ios::binary | std::ios::in), state_(stream_) {
    code_size_ = 2;
    while (1LL << code_size_ < word_size + code_size_ + 1) {
        ++code_size_;
    }

    code_size_ += word_size;

    code_size_ = (code_size_ + 7) / 8;

    word_size_ = (word_size + 7) / 8;
}

encoder::IEStream& encoder::IEStream::read(char* dest) {
    const auto tmp = new char[code_size_];

    std::memset(tmp, 0, code_size_);

    state_ = stream_.read(tmp, code_size_) ? true : false;

    auto bits = to_bits(tmp, code_size_);

    recover(bits);

    decode(bits);

    bits.resize(word_size_ * 8);

    to_bytes(bits, dest);

    return *this;
}

encoder::IEStream& encoder::IEStream::read(char* dest, const std::streamsize size) {
    std::memset(dest, 0, size);

    for (std::streamsize i = 0; state_ && i < size / word_size_; ++i) {
        read(dest + i * word_size_);
    }

    return *this;
}

encoder::IEStream& encoder::IEStream::ignore(const std::streamsize size, const bool raw) {
    if (raw) {
        stream_.ignore(size);
    }
    else {
        stream_.ignore(size / word_size_ * code_size_);
    }

    return *this;
}

encoder::IEStream& encoder::IEStream::close() {
    stream_.close();

    return *this;
}

encoder::IEStream::operator bool() const {
    return state_;
}

bool encoder::IEStream::operator!() const {
    return !state_;
}

encoder::OEStream::OEStream(const fs::path& file, const std::streamsize word_size)
    : stream_(file, std::ios::binary | std::ios::out | std::ios::app) {
    code_size_ = 2;
    while (1LL << code_size_ < word_size + code_size_ + 1) {
        ++code_size_;
    }

    code_size_ += word_size;

    code_size_ = (code_size_ + 7) / 8;

    word_size_ = (word_size + 7) / 8;
}

encoder::OEStream& encoder::OEStream::write(const char* src) {
    auto bits = to_bits(src, word_size_);

    encode(bits);

    const auto tmp = new char[code_size_];

    std::memset(tmp, 0, code_size_);

    to_bytes(bits, tmp);

    stream_.write(tmp, code_size_);

    stream_.flush();

    delete[] tmp;

    return *this;
}

encoder::OEStream& encoder::OEStream::write(const char* src, const std::streamsize size) {
    for (std::streamsize i = 0; i < size / word_size_; ++i) {
        write(src + i * word_size_);
    }

    return *this;
}

encoder::OEStream& encoder::OEStream::close() {
    stream_.close();

    return *this;
}
