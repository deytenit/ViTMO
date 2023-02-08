#pragma once

#include <vector>
#include <string>

namespace omfl_tools {
    std::vector<std::string> Split(const std::string& str, char delimiter);

    bool BeginsWith(const std::string& str, char pref);
    bool BeginsWith(const std::string& str, const std::string& pref);
    bool EndsWith(const std::string& str, char pref);
    bool EndsWith(const std::string& str, const std::string& pref);

    std::string Trim(const std::string& str);

    bool ValidateKey(const std::string& key);
    bool ValidateSection(const std::string& section);

    bool IsString(const std::string& value);
    bool IsFloat(const std::string& value);
    bool IsInt(const std::string& value);
}
