#include "expr/Variable.hpp"

Variable::Variable(std::string name) : m_name(std::move(name)) {}

Expression* Variable::clone() const {
    return new Variable(*this);
}

Exponential Variable::eval(const std::map<std::string, Exponential>& mapping) const {
    return mapping.count(m_name) != 0 ? mapping.at(m_name) : Exponential::NA;
}

std::string Variable::str() const {
    return m_name;
}