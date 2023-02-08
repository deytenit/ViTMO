#include <exceptions.h>

argument_parser::ArgumentParserException::ArgumentParserException(std::string name, std::string msg) :
    name_(std::move(name)), msg_(std::move(msg)) {
    what_ = "argument_parser error: " + name_ + " :: " + msg_;
}

const std::string& argument_parser::ArgumentParserException::name() const {
    return name_;
}

const std::string& argument_parser::ArgumentParserException::msg() const {
    return msg_;
}

const char* argument_parser::ArgumentParserException::what() const {
    return what_.c_str();
}
