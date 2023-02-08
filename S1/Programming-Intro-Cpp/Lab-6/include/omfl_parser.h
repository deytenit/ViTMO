#pragma once

#include <filesystem>
#include <unordered_map>
#include <map>
#include <string>

#include "omfl.h"

namespace omfl {
    namespace fs = std::filesystem;

    Section Parse(std::istream& in);
    Section Parse(const fs::path& path);
    Section Parse(const std::string& str);
    Section Parse(const std::unordered_map<std::string, std::string>& map);

    Value ParseValue(const std::string& value);

    void Stringify(const Section& section, std::ostream& out);
    void Stringify(const Section& section, const fs::path& path);
    void Stringify(const Section& section, const std::string& str);
    void Stringify(const Section& section, std::map<std::string, std::string>& map);

    void Jsonify(const Section& section, std::ostream& out, bool prettier = true);
    void Jsonify(const Section& section, const fs::path& path, bool prettier = true);
    void Jsonify(const Section& section, const std::string& str, bool prettier = true);

    std::string StringifyValue(const Value& value);
}
