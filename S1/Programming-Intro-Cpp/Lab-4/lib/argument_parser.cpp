#include <argument_parser.h>
#include <iterator>
#include <stdexcept>
#include <vector>

ArgumentParser::ArgumentParser(std::list<Option> options): options_(std::move(options)) {}

void ArgumentParser::parse(const int argc, char** argv) {
    exec_ = argv[0];

    std::vector<std::string> tokens = tokenize(argc, argv);

    for (size_t i = 1; i < tokens.size(); ++i) {
        if (begins_with(tokens[i], "--")) {
            std::string arg = tokens[i].substr(2);

            if (auto it = find_option(arg); it == options_.cend()) {
                arguments_.push_back({kUndefinedOption, {tokens[i], false}});
            }
            else if (it->type == kFlag) {
                arguments_.push_back({*it, {"", false}});
            }
            else if (it->type == kOptionalStore) {
                if (i + 1 == tokens.size() || begins_with(tokens[i + 1], '-')) {
                    arguments_.push_back({*it, {"", false}});
                }
                else {
                    arguments_.push_back({*it, {tokens[++i], true}});
                }
            }
            else {
                if (i + 1 == tokens.size()) {
                    throw std::runtime_error("ArgumentParser::parse error: No argument for option provided.");
                }
                arguments_.push_back({*it, {tokens[++i], true}});
            }
        }
        else if (begins_with(tokens[i], '-')) {
            std::string arg = tokens[i].substr(1);
            for (size_t j = 0; j < arg.size(); ++j) {

                if (auto it = find_option(arg[j]); it == options_.cend()) {
                    arguments_.push_back({kUndefinedOption, {std::string({'-', arg[j]}), false}});
                }
                else if (it->type == kFlag) {
                    arguments_.push_back({*it, {"", false}});
                }
                else if (it->type == kOptionalStore) {
                    if (j + 1 < arg.size() || i + 1 == tokens.size() || begins_with(tokens[i + 1], '-')) {
                        arguments_.push_back({*it, {"", false}});
                    }
                    else {
                        arguments_.push_back({*it, {tokens[++i], true}});
                    }
                }
                else {
                    if (j + 1 < arg.size() || i + 1 == tokens.size()) {
                        throw std::runtime_error("ArgumentParser::parse error: No argument for option provided.");
                    }
                    arguments_.push_back({*it, {tokens[++i], true}});
                }
            }
        }
        else {
            arguments_.push_back({kUnresolvedOption, {tokens[i], true}});
        }
    }
}

int ArgumentParser::next() {
    return args_pos_ == arguments_.size() ? -1 : arguments_[args_pos_++].option.value;
}

ArgumentParser::ArgumentValue ArgumentParser::value() const {
    return args_pos_ > 0 ? arguments_[args_pos_ - 1].value : ArgumentValue{};
}

std::vector<std::string> ArgumentParser::tokenize(const int argc, char** argv) {
    std::vector<std::string> result;

    for (int i = 0; i < argc; ++i) {
        std::string token = argv[i];

        if (const std::string::size_type eq_pos = token.find('='); eq_pos == std::string::npos) {
            result.push_back(token);
        }
        else {
            result.push_back(token.substr(0, eq_pos));
            result.push_back(token.substr(eq_pos + 1));
        }
    }

    result.shrink_to_fit();

    return result;
}

bool ArgumentParser::begins_with(const std::string& base, const char pref) {
    return !base.empty() && base[0] == pref;
}

bool ArgumentParser::begins_with(const std::string& base, const std::string& pref) {
    if (base.size() < pref.size()) {
        return false;
    }

    for (size_t i = 0; i < pref.size(); ++i) {
        if (base[i] != pref[i]) {
            return false;
        }
    }

    return true;
}

std::list<ArgumentParser::Option>::const_iterator ArgumentParser::find_option(const char& key) const {
    return std::find_if(options_.begin(), options_.end(), [&](const Option& item)
    {
        return key == item.shortcut;
    });
}

std::list<ArgumentParser::Option>::const_iterator ArgumentParser::find_option(const std::string& key) const {
    return std::find_if(options_.begin(), options_.end(), [&](const Option& item)
    {
        return key == item.key;
    });
}
