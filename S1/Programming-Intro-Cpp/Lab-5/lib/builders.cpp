#include <builders.h>

using namespace argument_parser;

OptionBuilder::OptionBuilder(std::unique_ptr<Option>& ptr)
    : argument_(ptr) {}

template <class T>
MultipleArgumentBuilder<T>::MultipleArgumentBuilder(
    std::unique_ptr<Option>& ptr)
    : OptionBuilder(ptr) {}

template <class T>
MultipleArgumentBuilder<T>& MultipleArgumentBuilder<T>::store(
    std::vector<T>& store) {
    dynamic_cast<MultipleArgument<T>*>(argument_.get())->store_ = &store;
    return *this;
}

template <class T>
MultipleArgumentBuilder<T>& MultipleArgumentBuilder<T>::positional() {
    dynamic_cast<MultipleArgument<T>*>(argument_.get())->positional_ = true;
    return *this;
}

template <class T>
SingleArgumentBuilder<T>::SingleArgumentBuilder(std::unique_ptr<Option>& ptr)
    : OptionBuilder(ptr) {}

template struct MultipleArgumentBuilder<int>;
template struct MultipleArgumentBuilder<std::string>;

template <class T>
SingleArgumentBuilder<T>& SingleArgumentBuilder<T>::set_default(
    const T& value) {
    dynamic_cast<SingleArgument<T>*>(argument_.get())->default_value_ = value;
    *dynamic_cast<SingleArgument<T>*>(argument_.get())->store_ = value;
    return *this;
}

template <class T>
SingleArgumentBuilder<T>& SingleArgumentBuilder<T>::store(T& store) {
    dynamic_cast<SingleArgument<T>*>(argument_.get())->store_ = &store;
    return *this;
}

template <class T>
SingleArgumentBuilder<T>& SingleArgumentBuilder<T>::positional() {
    dynamic_cast<SingleArgument<T>*>(argument_.get())->positional_ = true;
    return *this;
}

template <class T>
MultipleArgumentBuilder<T> SingleArgumentBuilder<T>::multiple() {
    argument_.reset(new MultipleArgument<T>(
        argument_->shortcut(),
        argument_->name(),
        argument_->info(),
        0
    ));
    return MultipleArgumentBuilder<T>(argument_);
}

template <class T>
MultipleArgumentBuilder<T> SingleArgumentBuilder<T>::multiple(
    const size_t min_size) {
    argument_.reset(new MultipleArgument<T>(
        argument_->shortcut(),
        argument_->name(),
        argument_->info(),
        min_size
    ));
    return MultipleArgumentBuilder<T>(argument_);
}

template struct SingleArgumentBuilder<int>;
template struct SingleArgumentBuilder<std::string>;

SingleArgumentBuilder<bool>::SingleArgumentBuilder(std::unique_ptr<Option>& ptr) : OptionBuilder(ptr) {
    set_default(false);
}

SingleArgumentBuilder<bool>& SingleArgumentBuilder<bool>::set_default(
    bool value) {
    dynamic_cast<SingleArgument<bool>*>(argument_.get())->default_value_ =
        value;
    *dynamic_cast<SingleArgument<bool>*>(argument_.get())->store_ = value;
    return *this;
}

SingleArgumentBuilder<bool>& SingleArgumentBuilder<bool>::store(bool& store) {
    dynamic_cast<SingleArgument<bool>*>(argument_.get())->store_ = &store;
    return *this;
}
