#include <options.h>
#include <string>

using namespace argument_parser;

Option::Option(std::string name) : name_(std::move(name)) {}

Option::Option(std::string name, std::string info)
    : name_(std::move(name)), info_(std::move(info)) {}

Option::Option(const char shortcut, std::string name)
    : shortcut_(shortcut), name_(std::move(name)) {}

Option::Option(const char shortcut, std::string name, std::string info)
    : shortcut_(shortcut), name_(std::move(name)), info_(std::move(info)) {}

Option::Option(const Option& option): Option(option.shortcut_, option.name_, option.info_) {}

Option::Option(Option&& option) noexcept: shortcut_(option.shortcut_),
                                                name_(std::move(option.name_)),
                                                info_(std::move(option.info_)) {}

Option& Option::operator=(const Option& option) {
    if (this == &option) {
        return *this;
    }

    shortcut_ = option.shortcut_;
    name_ = option.name_;
    info_ = option.info_;

    return *this;
}

Option& Option::operator=(Option&& option) noexcept {
    if (this == &option) {
        return *this;
    }

    shortcut_ = option.shortcut_;
    name_ = std::move(option.name_);
    info_ = std::move(option.info_);

    return *this;
}

char Option::shortcut() const {
    return shortcut_;
}

const std::string& Option::name() const {
    return name_;
}

const std::string& Option::info() const {
    return info_;
}

void Option::from_string(const std::string& str) {}

bool Option::has_default_value() const {
    return false;
}

template <class T>
SingleArgument<T>::SingleArgument(std::string name) : Option(std::move(name)) {}

template <class T>
SingleArgument<T>::SingleArgument(const std::string name, std::string info)
    : Option(name, std::move(info)) {}

template <class T>
SingleArgument<T>::SingleArgument(const char shortcut, std::string name)
    : Option(shortcut, std::move(name)) {}

template <class T>
SingleArgument<T>::SingleArgument(const char shortcut, std::string name,
                                  std::string info)
    : Option(shortcut, std::move(name), std::move(info)) {}

template class SingleArgument<bool>;
template class SingleArgument<int>;
template class SingleArgument<std::string>;

template <>
void SingleArgument<bool>::from_string(const std::string& str) {
    std::string tmp = str;
    for (char& chr : tmp) {
        chr = static_cast<char>(std::tolower(chr));
    }

    *store_ = tmp == "toggle" && has_default_value()
                 ? !default_value_.value()
                 : tmp == "true" || !has_default_value()
                 ? true
                 : false;
}

template <class T>
bool SingleArgument<T>::has_default_value() const {
    return default_value_.has_value();
}

template <>
void SingleArgument<int>::from_string(const std::string& str) {
    *store_ = std::stoi(str);
}

template <>
void SingleArgument<std::string>::from_string(const std::string& str) {
    *store_ = std::string(str);
}

template <class T>
MultipleArgument<T>::MultipleArgument(std::string name, const size_t min_size)
    : Option(std::move(name)), min_size_(min_size) {
    store_->reserve(min_size_);
}

template <class T>
MultipleArgument<T>::MultipleArgument(const std::string name, std::string info,
                                      const size_t min_size)
    : Option(name, std::move(info)), min_size_(min_size) {
    store_->reserve(min_size_);
}

template <class T>
MultipleArgument<T>::MultipleArgument(const char shortcut, std::string name,
                                      const size_t min_size)
    : Option(shortcut, std::move(name)), min_size_(min_size) {
    store_->reserve(min_size_);
}

template <class T>
MultipleArgument<T>::MultipleArgument(const char shortcut, std::string name,
                                      std::string info, const size_t min_size)
    : Option(shortcut, std::move(name), std::move(info)), min_size_(min_size) {
    store_->reserve(min_size_);
}

template <class T>
size_t MultipleArgument<T>::min_size() const {
    return min_size_;
}

template <class T>
bool MultipleArgument<T>::has_default_value() const {
    return false;
}

template class MultipleArgument<int>;
template class MultipleArgument<std::string>;

template <>
void MultipleArgument<int>::from_string(const std::string& str) {
    store_->push_back(std::stoi(str));
}

template <>
void MultipleArgument<std::string>::from_string(const std::string& str) {
    store_->push_back(str);
}
