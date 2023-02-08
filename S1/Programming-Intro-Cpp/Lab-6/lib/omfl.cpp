#include <omfl.h>
#include <misc.h>

#include <algorithm>

using namespace omfl;
using namespace omfl_tools;

template <class T>
bool Omfl::Is() const {
    return typeid(T) == Type();
}

template <class T>
const T& Omfl::As() const {
    return *static_cast<const T*>(Raw());
}

bool Omfl::IsSection() const {
    return dynamic_cast<const Section*>(this);
}

bool Omfl::IsValue() const {
    return dynamic_cast<const Value*>(this);
}

Section& Omfl::AsSection() const {
    return *const_cast<Section*>(dynamic_cast<const Section*>(this));
}

Value& Omfl::AsValue() const {
    return *const_cast<Value*>(dynamic_cast<const Value*>(this));
}

template <class T>
const T& Omfl::AsOrDefault(const T& value) const {
    if (Is<T>()) {
        return *static_cast<const T*>(Raw());
    }

    return value;
}

namespace omfl {
    template const bool& Omfl::AsOrDefault<bool>(const bool&) const;
    template const int& Omfl::AsOrDefault<int>(const int&) const;
    template const float& Omfl::AsOrDefault<float>(const float&) const;
    template const std::string& Omfl::AsOrDefault<std::string>(const std::string&) const;
}

Value::Value(std::any value) : data_(std::move(value)) {}

bool Value::Valid() const {
    return true;
}

bool Value::Has(const std::string& name) const {
    return false;
}

const Omfl& Value::Get(const std::string& name) const {
    return *this;
}

template <class T>
void Value::Set(const T& value) {
    data_ = value;
}

const Omfl& Value::operator[](const size_t index) const {
    try {
        return std::any_cast<const Array&>(data_).at(index);
    }
    catch (...) {
        return *this;
    }
}

const void* Value::Raw() const {
    return &data_;
}

const std::type_info& Value::Type() const {
    return data_.type();
}

bool Section::Valid() const {
    return std::all_of(
        data_.begin(), data_.end(), [](const auto& item) {
            return item.second->Valid();
        }
    );
}

bool Section::Has(const std::string& name) const {
    return data_.count(name) != 0;
}

const Omfl& Section::Get(const std::string& name) const {
    if (const size_t pos = name.find('.'); pos != std::string::npos) {
        return data_.at(name.substr(0, pos))->Get(name.substr(pos + 1));
    }

    return *data_.at(name);
}

void Section::SetValue(const std::string& name, Value value) {
    data_[name] = std::make_unique<Value>(std::move(value));
}

void Section::SetSection(const std::string& name, Section section) {
    data_[name] = std::make_unique<Section>(std::move(section));
}

const Omfl& Section::operator[](size_t index) const {
    return *this;
}

const void* Section::Raw() const {
    return &data_;
}

const std::type_info& Section::Type() const {
    return typeid(SectionContainer);
}
