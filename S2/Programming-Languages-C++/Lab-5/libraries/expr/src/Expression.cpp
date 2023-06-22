#include "expr/Expression.hpp"

#include <sstream>

// --------------------UnaryExpression

Exponential UnaryExpression::eval(const std::map<std::string, Exponential>& mapping) const {
    return operator()(m_value->eval(mapping));
}

std::string UnaryExpression::str() const {
    std::stringstream ss;
    ss << operation() << '(' << m_value->str() << ')';
    return ss.str();
}

UnaryExpression::UnaryExpression(const Expression& value) : m_value(value.clone()) {}

// --------------------BinaryExpression

Exponential BinaryExpression::eval(const std::map<std::string, Exponential>& mapping) const {
    return operator()(m_lhs->eval(mapping), m_rhs->eval(mapping));
}

std::string BinaryExpression::str() const {
    std::stringstream ss;
    ss << '(' << m_lhs->str() << ' ' << operation() << ' ' << m_rhs->str() << ')';
    return ss.str();
}

BinaryExpression::BinaryExpression(const Expression& lhs, const Expression& rhs)
    : m_lhs(lhs.clone()), m_rhs(rhs.clone()) {}

// --------------------NonMember

std::ostream& operator<<(std::ostream& out, const Expression& object) {
    return out << object.str();
}