#include <misc.h>

#include <algorithm>

std::vector<std::string> omfl_tools::Split(const std::string& str, const char delimiter) {
    std::vector<std::string> result(1);

    for (const char c : str) {
        if (c == delimiter && !result.back().empty()) {
            result.emplace_back();
        }
        else {
            result.back() += c;
        }
    }

    if (result.back().empty()) {
        result.pop_back();
    }

    return result;
}

bool omfl_tools::BeginsWith(const std::string& str, const char pref) {
    return !str.empty() && str.front() == pref;
}

bool omfl_tools::BeginsWith(const std::string& str, const std::string& pref) {
    return str.size() >= pref.size() && str.substr(0, pref.size()) == pref;
}

bool omfl_tools::EndsWith(const std::string& str, const char pref) {
    return !str.empty() && str.back() == pref;
}

bool omfl_tools::EndsWith(const std::string& str, const std::string& pref) {
    return str.size() >= pref.size() && str.substr(str.size() - pref.size()) == pref;
}

std::string omfl_tools::Trim(const std::string& str) {
    size_t l = 0;
    size_t r = str.size();

    while (l < str.size() && str[l] == ' ') {
        ++l;
    }

    while (r > 0 && str[r - 1] == ' ') {
        --r;
    }

    if (l > r) {
        return "";
    }

    return str.substr(l, r - l);
}

bool omfl_tools::ValidateKey(const std::string& key) {
    return !key.empty() && std::all_of(
        key.begin(), key.end(), [](const char chr) {
            return ('a' <= std::tolower(chr) && std::tolower(chr) <= 'z')
                || ('0' <= chr && chr <= '9')
                || chr == '_'
                || chr == '-';
        }
    );
}

bool omfl_tools::ValidateSection(const std::string& section) {
    return !section.empty()
        && !BeginsWith(section, '.')
        && !EndsWith(section, '.')
        && std::all_of(
            section.begin(), section.end(), [](const char chr) {
                return ('a' <= std::tolower(chr) && std::tolower(chr) <= 'z')
                    || ('0' <= chr && chr <= '9')
                    || chr == '_'
                    || chr == '-'
                    || chr == '.';
            }
        );
}

bool omfl_tools::IsString(const std::string& value) {
    return BeginsWith(value, '"') && EndsWith(value, '"')
        && std::count(value.begin(), value.end(), '"') == 2;
}

bool omfl_tools::IsFloat(const std::string& value) {
    if (value.empty() || (value[0] != '-' && value[0] != '+' && !std::isdigit(value[0]))) {
        return false;
    }

    bool point = false;

    for (size_t i = 1; i < value.size(); ++i) {
        if ((!std::isdigit(value[i]) && value[i] != '.') || (point && value[i] == '.')) {
            return false;
        }

        if (value[i] == '.') {
            if (i + 1 == value.size() || i == 0 || !std::isdigit(value[i + 1]) || !std::isdigit(value[i - 1])) {
                return false;
            }

            point = true;
        }
    }

    return true;
}

bool omfl_tools::IsInt(const std::string& value) {
    if (value.empty() || (value[0] != '-' && value[0] != '+' && !std::isdigit(value[0]))) {
        return false;
    }

    return std::all_of(
        value.begin() + 1, value.end(), [](const char chr) {
            return std::isdigit(chr);
        }
    );
}
