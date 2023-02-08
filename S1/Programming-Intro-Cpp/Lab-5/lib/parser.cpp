#include <algorithm>
#include <parser.h>
#include <memory>
#include <stdexcept>
#include <string>

using namespace argument_parser;

Parser::Parser(std::string name) : name_(std::move(name)) {}

void Parser::parse(const int argc, char** argv) const {
    parse(std::vector<std::string>(argv, argv + argc));
}

void Parser::parse(const std::vector<std::string>& args) const {
    std::list<std::string> tokens = tokenize(args);

    path_ = tokens.front();
    tokens.pop_front();

    for (const std::unique_ptr<Option>& option : options_) {
        if (option->positional_) {
            continue;
        }

        auto it = tokens.cend();

        bool resolved = false;

        while ((it = find_token(tokens, *option)) != tokens.cend()) {

            resolved = true;

            if (dynamic_cast<SingleArgument<bool>*>(option.get())) {
                option->from_string("toggle");

                tokens.erase(it);
            }
            else if (
                auto it_arg = it; ++it_arg != tokens.end() && !begins_with(*it_arg, '-')
            ) {
                option->from_string(*it_arg);

                tokens.erase(it);
                tokens.erase(it_arg);
            }
            else {
                throw ArgumentParserException(option->name(), "parse: Option without argument.");
            }
        }

        if (!resolved && !option->has_default_value()) {
            throw ArgumentParserException(option->name(),"parse: No option or default value was provided.");
        }
    }

    if (help_.has_value()) {
        if (const auto it = find_token(tokens, help_.value()); it != tokens.cend()) {
            *help_->store_ = true;

            tokens.erase(it);
        }
    }

    for (const std::string& token : tokens) {
        if (begins_with(token, '-')) {
            throw ArgumentParserException(token,"parse: Unknown option.");
        }

        std::for_each(options_.begin(), options_.end(), [&](auto& option)
        {
            if (option->positional_) {
                option->from_string(token);
            }
        });
    }

    for (const std::unique_ptr<Option>& option : options_) {
        if (const auto casted = dynamic_cast<MultipleArgument<int>*>(option.get())) {
            if (casted->min_size() > casted->store_->size()) {
                throw ArgumentParserException(
                    option->name(), "parse: Not enough arguments were provided.");
            }
        }

        if (auto casted =
            dynamic_cast<MultipleArgument<std::string>*>(option.get())) {
            if (casted->min_size() > casted->store_->size()) {
                throw ArgumentParserException(
                    option->name(),
                    "parse: Not enough arguments were provided.");
            }
        }
    }
}

void Parser::add_help(const std::string& name) {
    help_ = SingleArgument<bool>(name);
}

void Parser::add_help(const std::string& name, const std::string& info) {
    help_ = SingleArgument<bool>(name, info);
}

void Parser::add_help(const char shortcut, const std::string& name) {
    help_ = SingleArgument<bool>(shortcut, name);
}

void Parser::add_help(const char shortcut, const std::string& name, const std::string& info) {
    help_ = SingleArgument<bool>(shortcut, name, info);
}

std::string Parser::get_help() const {
    if (!help_.has_value()) {
        throw ArgumentParserException("help", "get_help: Unknown option.");
    }

    std::string result = name_ + '\n';

    result += help_->info() + "\n\n";

    for (const std::unique_ptr<Option>& option : options_) {
        if (option->shortcut() != kUndefinedShortcut) {
            result += "-" + std::string{option->shortcut()} + ", ";
        }
        else {
            result += "    ";
        }

        result += "--" + option->name();

        if (auto casted = dynamic_cast<SingleArgument<bool>*>(option.get())) {
            result += ", " + casted->info();
            if (casted->has_default_value() && casted->default_value_.value()) {
                result += " [default = ";
                result += casted->default_value_.value() ? "true" : "false";
                result += "]";
            }
            result += '\n';
            continue;
        }

        if (auto casted = dynamic_cast<SingleArgument<int>*>(option.get())) {
            result += "=<int>, " + casted->info();
            if (casted->has_default_value()) {
                result += " [default = ";
                result += std::to_string(casted->default_value_.value());
                result += "]";
            }
            result += '\n';
            continue;
        }

        if (auto casted = dynamic_cast<SingleArgument<std::string>*>(option.get())) {
            result += "=<int>, " + casted->info();
            if (casted->has_default_value()) {
                result += " [default = ";
                result += casted->default_value_.value();
                result += "]";
            }
            result += '\n';
            continue;
        }

        if (auto casted = dynamic_cast<MultipleArgument<int>*>(option.get())) {
            result += "=<int>, " + casted->info();
            result += " [repeated, min args = ";
            result += std::to_string(casted->min_size());
            result += "]";
            result += '\n';
            continue;
        }

        if (auto casted =
            dynamic_cast<MultipleArgument<std::string>*>(option.get())) {
            result += "=<string>, " + casted->info();
            result += " [repeated, min args = ";
            result += std::to_string(casted->min_size());
            result += "]";
            result += '\n';
        }
    }

    result += '\n';

    if (help_->shortcut() != kUndefinedShortcut) {
        result += "-" + std::string{help_->shortcut()};
        result += ", ";
    }
    else {
        result += "    ";
    }

    result += "--" + help_->name();
    result += " Display this help and exit\n";

    return result;
}

SingleArgumentBuilder<bool> Parser::add_flag(const std::string& name) {
    options_.push_back(std::make_unique<SingleArgument<bool>>(name));
    return SingleArgumentBuilder<bool>{options_.back()};
}

SingleArgumentBuilder<bool> Parser::add_flag(
    const std::string& name,
    const std::string& info
) {
    options_.push_back(std::make_unique<SingleArgument<bool>>(name, info));
    return SingleArgumentBuilder<bool>{options_.back()};
}

SingleArgumentBuilder<bool> Parser::add_flag(
    const char shortcut,
    const std::string& name
) {
    options_.push_back(std::make_unique<SingleArgument<bool>>(shortcut, name));
    return SingleArgumentBuilder<bool>{options_.back()};
}

SingleArgumentBuilder<bool> Parser::add_flag(
    const char shortcut,
    const std::string& name,
    const std::string& info
) {
    options_.push_back(std::make_unique<SingleArgument<bool>>(shortcut, name, info));
    return SingleArgumentBuilder<bool>{options_.back()};
}

bool Parser::get_flag(const std::string& name) const {
    const auto it = find_option(name);

    if (it == options_.end()) {
        throw ArgumentParserException(name, "get_flag: Unknown option.");
    }

    if (const SingleArgument<bool>* option
        = dynamic_cast<SingleArgument<bool>*>(it->get())
    ) {
        return *option->store_;
    }
    throw ArgumentParserException(name, "get_flag: Not a flag.");
}

SingleArgumentBuilder<int> Parser::add_int_option(const std::string& name) {
    options_.push_back(
        std::make_unique<SingleArgument<int>>(name));
    return SingleArgumentBuilder<int>{options_.back()};
}

SingleArgumentBuilder<int> Parser::add_int_option(
    const std::string& name,
    const std::string& info
) {
    options_.push_back(
        std::make_unique<SingleArgument<int>>(name, info));
    return SingleArgumentBuilder<int>{options_.back()};
}

SingleArgumentBuilder<int> Parser::add_int_option(
    const char shortcut,
    const std::string& name
) {
    options_.push_back(
        std::make_unique<SingleArgument<int>>(shortcut, name));
    return SingleArgumentBuilder<int>{options_.back()};
}

SingleArgumentBuilder<int> Parser::add_int_option(
    const char shortcut,
    const std::string& name,
    const std::string& info
) {
    options_.push_back(
        std::make_unique<SingleArgument<int>>(shortcut, name, info));
    return SingleArgumentBuilder<int>{options_.back()};
}

int Parser::get_int_option(const std::string& name) const {
    const auto it = find_option(name);

    if (it == options_.end()) {
        throw ArgumentParserException(name, "get_int_option: Unknown option.");
    }

    if (const SingleArgument<int>* option =
        dynamic_cast<SingleArgument<int>*>(it->get())
    ) {
        return *option->store_;
    }

    throw ArgumentParserException(name, "get_int_option: Not a valid single int option.");
}

int Parser::get_int_option(const std::string& name, const size_t idx) const {
    const auto it = find_option(name);

    if (it == options_.end()) {
        throw ArgumentParserException(name, "get_int_option: Unknown option.");
    }

    if (const MultipleArgument<int>* option =
        dynamic_cast<MultipleArgument<int>*>(it->get())
    ) {
        return option->store_->at(idx);
    }
    throw ArgumentParserException(name, "get_int_option: Not a valid multiple int option.");
}

SingleArgumentBuilder<std::string> Parser::add_string_option(const std::string& name) {
    options_.push_back(
        std::make_unique<SingleArgument<std::string>>(name));
    return SingleArgumentBuilder<std::string>{options_.back()};
}

SingleArgumentBuilder<std::string> Parser::add_string_option(
    const std::string& name,
    const std::string& info
) {
    options_.push_back(
        std::make_unique<SingleArgument<std::string>>(name, info));
    return SingleArgumentBuilder<std::string>{options_.back()};
}

SingleArgumentBuilder<std::string> Parser::add_string_option(
    const char shortcut,
    const std::string& name
) {
    options_.push_back(
        std::make_unique<SingleArgument<std::string>>(shortcut, name));
    return SingleArgumentBuilder<std::string>{options_.back()};
}

SingleArgumentBuilder<std::string> Parser::add_string_option(
    const char shortcut,
    const std::string& name,
    const std::string& info
) {
    options_.push_back(
        std::make_unique<SingleArgument<std::string>>(shortcut, name, info));
    return SingleArgumentBuilder<std::string>{options_.back()};
}

const std::string& Parser::get_string_option(const std::string& name) const {
    const auto it = find_option(name);

    if (it == options_.end()) {
        throw ArgumentParserException(name, "get_string_option: Unknown option.");
    }

    if (const SingleArgument<std::string>* option =
        dynamic_cast<SingleArgument<std::string>*>(it->get())
    ) {
        return *option->store_;
    }
    throw ArgumentParserException(
        name, "get_string_option: Not a valid single string option.");
}

const std::string& Parser::get_string_option(const std::string& name, const size_t idx) const {
    const auto it = find_option(name);

    if (it == options_.end()) {
        throw ArgumentParserException(name, "get_string_option: Unknown option.");
    }

    if (const MultipleArgument<std::string>* option =
        dynamic_cast<MultipleArgument<std::string>*>(it->get())
    ) {
        return option->store_->at(idx);
    }
    throw ArgumentParserException(
        name, "get_string_option: Not a valid multiple string option.");
}

std::list<std::unique_ptr<Option>>::const_iterator Parser::find_option(const std::string& key) const {
    return std::find_if(options_.begin(), options_.end(), [&](const std::unique_ptr<Option>& item)
    {
        return key == item->name();
    });
}

std::list<std::string>::const_iterator Parser::find_token(const std::list<std::string>& tokens, const Option& option) {
    auto it = tokens.end();

    if (option.shortcut() != kUndefinedShortcut) {
        it = std::find(tokens.begin(), tokens.end(),
                       "-" + std::string{option.shortcut()});
    }

    if (it == tokens.cend()) {
        it = std::find(tokens.begin(), tokens.end(), "--" + option.name());
    }

    return it;
}

std::list<std::string> Parser::tokenize(const std::vector<std::string>& args) {
    std::list<std::string> result;

    for (const std::string& token : args) {
        if (begins_with(token, "--")) {
            if (const std::string::size_type eq_pos = token.find('='); eq_pos == std::string::npos) {
                result.push_back(token);
            }
            else {
                result.push_back(token.substr(0, eq_pos));
                result.push_back(token.substr(eq_pos + 1));
            }
        }
        else if (begins_with(token, '-')) {
            for (const char arg : token) {
                if (arg == '=') {
                    break;
                }
                if (arg != '-' && arg != kUndefinedShortcut) {
                    result.emplace_back("-" + std::string{arg});
                }
            }

            if (const std::string::size_type eq_pos = token.find('=');
                eq_pos != std::string::npos) {
                result.push_back(token.substr(eq_pos + 1));
            }
        }
        else {
            result.push_back(token);
        }
    }

    return result;
}

bool Parser::begins_with(const std::string& base, const char pref) {
    return !base.empty() && base[0] == pref;
}

bool Parser::begins_with(const std::string& base, const std::string& pref) {
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
