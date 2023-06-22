#include "expr/Const.hpp"

Const::Const(Exponential value) : m_value(value) {}

Expression* Const::clone() const {
    return new Const(*this);
}

Exponential Const::eval(const std::map<std::string, Exponential>&) const {
    return m_value;
}

std::string Const::str() const {
    return m_value.str();
}