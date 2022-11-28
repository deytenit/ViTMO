#pragma once

#include <string>
#include <list>
#include <utility>
#include <vector>
#include <stdexcept>
#include <algorithm>

class ArgumentParser {
public:
    enum OptionType {
        kFlag,
        kStore,
        kOptionalStore
    };

    struct Option {
        std::string key;
        char shortcut{};
        OptionType type;
        int value{};

        Option(std::string key, const char shortcut, const OptionType type, const int value) : key(std::move(key)), shortcut(shortcut), type(type), value(value) {}
    };

    const Option kUnresolvedOption = {"!", '!', kFlag, '!'};
    const Option kUndefinedOption = {"?", '?', kFlag, '?'};

    using ArgumentValue = std::pair<std::string, bool>;

    struct Argument {
        Option option;
        ArgumentValue value;
    };

private:
    std::string exec_;

    std::list<Option> options_;
    std::vector<Argument> arguments_;
    size_t args_pos_ = 0;

public:
    explicit ArgumentParser(std::list<Option> options);

    void parse(int argc, char** argv);

    int next();

    [[nodiscard]] ArgumentValue value() const;

private:
    static std::vector<std::string> tokenize(int argc, char** argv);

    static bool begins_with(const std::string& base, char pref);

    static bool begins_with(const std::string& base, const std::string& pref);

    [[nodiscard]] std::list<Option>::const_iterator find_option(const char& key) const;

    [[nodiscard]] std::list<Option>::const_iterator find_option(const std::string& key) const;
};
