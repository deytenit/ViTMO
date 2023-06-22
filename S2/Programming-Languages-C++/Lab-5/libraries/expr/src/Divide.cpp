#include "expr/Divide.hpp"

Divide::Divide(const Expression& lhs, const Expression& rhs) : BinaryExpression(lhs, rhs) {}

Expression* Divide::clone() const {
    return new Divide(*this);
}

char Divide::operation() const {
    return OPERATION;
}

Exponential Divide::operator()(const Exponential& lhs, const Exponential& rhs) const {
    return lhs / rhs;
}

Divide operator/(const Expression& lhs, const Expression& rhs) {
    return {lhs, rhs};
}