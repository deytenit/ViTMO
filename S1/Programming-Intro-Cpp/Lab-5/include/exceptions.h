#pragma once

#include <exception>
#include <string>

namespace argument_parser {
class ArgumentParserException : public std::exception {
private:
    std::string name_;
    std::string msg_;

    std::string what_;

public:
    ArgumentParserException(std::string name, std::string msg);

    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const std::string& msg() const;
    [[nodiscard]] char const* what() const override;
};
}