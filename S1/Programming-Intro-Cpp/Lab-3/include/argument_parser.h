#pragma once

#include <string>
#include <list>
#include <vector>
#include <stdexcept> 

class argument_parser {
public:
    enum option_type {
        FLAG,
        STORE
    };

    struct option {
        std::string key;
        char shortcut;
        option_type type;
        int value;
    };
private:
    std::string exec_;

    std::list<option> options_;
    std::vector<std::string> args_;
    size_t args_pos_ = 0;
    std::vector<std::string> opts_;
    size_t opts_pos_ = 0;
public:
    argument_parser(std::list<option> options);

    void parse(int argc, char** argv);

    int next();

    [[nodiscard]] std::string value() const;

private:
    [[nodiscard]] std::list<option>::const_iterator find_option(const char& key) const {
        return std::find_if(options_.begin(), options_.end(), [&](const option& item) {
            return key == item.shortcut;
        });
    }

    [[nodiscard]] std::list<option>::const_iterator find_option(const std::string& key) const {
        return std::find_if(options_.begin(), options_.end(), [&](const option& item) {
            return key == item.key;
        });
    } 
};
