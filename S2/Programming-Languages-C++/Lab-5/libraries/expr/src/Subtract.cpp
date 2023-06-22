#include "expr/Subtract.hpp"

Subtract::Subtract(const Expression& lhs, const Expression& rhs) : BinaryExpression(lhs, rhs) {}

Expression* Subtract::clone() const {
    return new Subtract(*this);
}

char Subtract::operation() const {
    return OPERATION;
}

Exponential Subtract::operator()(const Exponential& lhs, const Exponential& rhs) const {
    return lhs - rhs;
}

Subtract operator-(const Expression& lhs, const Expression& rhs) {
    return {lhs, rhs};
}