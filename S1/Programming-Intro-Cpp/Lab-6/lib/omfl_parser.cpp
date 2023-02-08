#include <omfl_parser.h>
#include <misc.h>

#include <fstream>
#include <map>
#include <sstream>
#include <unordered_set>

using namespace omfl_tools;

omfl::Section& MakeSections(omfl::Section& root, const std::string& nesting) {
    const size_t delimiter = nesting.find('.');

    if (delimiter == std::string::npos) {
        if (!root.Has(nesting)) {
            root.SetSection(nesting, omfl::Section());
        }

        return root.Get(nesting).AsSection();
    }

    const std::string section = nesting.substr(0, delimiter);
    const std::string next = nesting.substr(delimiter + 1);

    if (!root.Has(section)) {
        root.SetSection(section, omfl::Section());
    }

    return MakeSections(root.Get(section).AsSection(), next);
}


omfl::Section omfl::Parse(std::istream& in) {
    std::string line;

    std::unordered_map<std::string, std::string> tokens;

    std::string section;

    while (std::getline(in, line)) {
        line = Trim(line);

        if (const size_t pos = line.find('#'); pos != std::string::npos) {
            line = line.substr(0, pos);
        }

        if (line.empty()) {
            continue;
        }

        if (BeginsWith(line, '[') && EndsWith(line, ']')) {
            section = line.substr(1, line.size() - 2);

            if (!ValidateSection(section)) {
                throw std::invalid_argument("omfl::Parse error: Wrong section name was passed: " + section);
            }

            continue;
        }

        if (const size_t pos = line.find('='); pos != std::string::npos) {
            std::string key = section + "::" + Trim(line.substr(0, pos));
            std::string value = Trim(line.substr(pos + 1));

            if (tokens.count(key) == 0) {
                tokens.insert({key, value});
            }
            else {
                throw std::invalid_argument("omfl::parser error: Duplicate of key: " + key);
            }

            continue;
        }

        throw std::invalid_argument("omfl::parse error: The following line cannot be parsed:\n" + line);
    }

    return Parse(tokens);
}

omfl::Section omfl::Parse(const fs::path& path) {
    std::ifstream in(path, std::ios::in);

    return Parse(in);
}

omfl::Section omfl::Parse(const std::string& str) {
    std::istringstream in{str};

    return Parse(in);
}

omfl::Section omfl::Parse(const std::unordered_map<std::string, std::string>& map) {
    Section root;

    for (const auto& [fst, snd] : map) {
        const size_t delimiter = fst.find("::");

        std::string nesting = delimiter != std::string::npos ? fst.substr(0, delimiter) : "";
        std::string key = delimiter != std::string::npos ? fst.substr(delimiter + 2) : fst;

        if (!ValidateKey(key)) {
            throw std::invalid_argument("omfl::Parse error: Not a valid key name: " + key);
        }

        if (nesting.empty()) {
            root.SetValue(key, ParseValue(snd));
            continue;
        }

        if (!ValidateSection(nesting)) {
            throw std::invalid_argument("omfl::Parse error: Not a valid section name: " + nesting);
        }

        Section& section = MakeSections(root, nesting);

        section.SetValue(key, ParseValue(snd));
    }

    return root;
}

omfl::Value omfl::ParseValue(const std::string& value) {
    if (IsString(value)) {
        return Value{value.substr(1, value.size() - 2)};
    }

    if (BeginsWith(value, '[') && EndsWith(value, ']')) {
        std::vector<Value> result;
        std::string token;

        uint32_t depth = 0;

        for (size_t i = 1; i < value.size() - 1; ++i) {
            if (value[i] == '[') {
                ++depth;
            }
            else if (value[i] == ']') {
                --depth;
            }

            if (value[i] != ',' && value[i] != ' ' || depth != 0) {
                token += value[i];
            }
            else if (!token.empty()) {
                result.push_back(ParseValue(token));
                token = "";
            }
        }

        if (!token.empty()) {
            result.push_back(ParseValue(token));
            token = "";
        }

        return Value{result};
    }

    if (value == "true") {
        return Value{true};
    }

    if (value == "false") {
        return Value{false};
    }

    if (IsInt(value)) {
        return Value(std::stoi(value));
    }

    if (IsFloat(value)) {
        return Value(std::stof(value));
    }


    throw std::invalid_argument("omfl::ParseValue error: Illegal argument: " + value);
}

void RestoreSections(
    std::map<std::string, std::string>& map, const omfl::Section& section, const std::string& nesting
) {
    const auto& data = section.As<omfl::SectionContainer>();

    for (const auto& [name, value] : data) {
        if (value->IsSection()) {
            RestoreSections(
                map, value->AsSection(),
                nesting.empty() ? std::string{name} : nesting + "." + std::string{name}
            );
            continue;
        }

        map.insert({nesting + "::" += name, StringifyValue(value->AsValue())});
    }
}

void omfl::Stringify(const Section& section, std::ostream& out) {
    std::map<std::string, std::string> map;
    Stringify(section, map);

    std::string current;

    for (const auto& [location, value] : map) {
        const size_t pos = location.find("::");

        if (pos == std::string::npos) {
            throw std::runtime_error("omfl::Stringify error: Section::key is not correct: " + location);
        }

        if (const std::string tmp = location.substr(0, pos); tmp != current) {
            current = location.substr(0, pos);
            out << '\n' << '[' << current << ']' << '\n';
        }

        std::string key = location.substr(pos + 2);

        out << key << " = " << value << '\n';
    }
}

void omfl::Stringify(const Section& section, const fs::path& path) {
    std::ofstream out(path, std::ios::out | std::ios::trunc);

    Stringify(section, out);
}

void omfl::Stringify(const Section& section, const std::string& str) {
    std::ostringstream out{str};

    Stringify(section, out);
}

void omfl::Stringify(const Section& section, std::map<std::string, std::string>& map) {
    map.clear();

    const std::string nesting;

    RestoreSections(map, section, nesting);
}

void JsonRecursive(const omfl::Section& section, std::ostream& out, const uint32_t depth, const bool prettier) {
    const auto& data = section.As<omfl::SectionContainer>();
    const std::string padding = prettier ? std::string(depth, '\t') : "";
    const std::string end = prettier ? "\n" : "";

    for (auto it = data.begin(); it != data.end(); ++it) {
        if (auto& [name, value] = *it; value->IsSection()) {
            out << padding << '"' << name << '"' << ": {" << end;

            JsonRecursive(value->AsSection(), out, depth + 1, prettier);

            out << padding << "}";
        }
        else {
            out << padding << '"' << name << '"' << ": " << StringifyValue(value->AsValue());
        }

        if (it != --data.end()) {
            out << ',';
        }

        out << end;
    }
}

void omfl::Jsonify(const Section& section, std::ostream& out, const bool prettier) {
    const std::string end = prettier ? "\n" : "";

    out << '{' << end;
    JsonRecursive(section, out, 1, prettier);
    out << '}' << end;
}

void omfl::Jsonify(const Section& section, const fs::path& path, const bool prettier) {
    std::ofstream out(path, std::ios::out | std::ios::trunc);

    Jsonify(section, out, prettier);
}

void omfl::Jsonify(const Section& section, const std::string& str, const bool prettier) {
    std::ostringstream out{str};

    Jsonify(section, out, prettier);
}

std::string omfl::StringifyValue(const Value& value) {
    if (value.Is<std::string>()) {
        return "\"" + value.As<std::string>() + "\"";
    }

    if (value.Is<Array>()) {
        std::string result;

        const auto& data = value.As<Array>();

        result += "[ ";

        bool post_delimiter = false;

        for (const Value& item : data) {
            if (!post_delimiter) {
                post_delimiter = true;
            }
            else {
                result += ", ";
            }

            result += StringifyValue(item);
        }

        result += " ]";

        return result;
    }

    if (value.Is<bool>()) {
        return value.As<bool>() ? "true" : "false";
    }

    if (value.Is<int>()) {
        return std::to_string(value.As<int>());
    }

    if (value.Is<float>()) {
        return std::to_string(value.As<float>());
    }

    return "";
}
