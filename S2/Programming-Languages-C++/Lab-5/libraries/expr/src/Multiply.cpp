#include "expr/Multiply.hpp"

Multiply::Multiply(const Expression& lhs, const Expression& rhs) : BinaryExpression(lhs, rhs) {}

Expression* Multiply::clone() const {
    return new Multiply(*this);
}

char Multiply::operation() const {
    return OPERATION;
}

Exponential Multiply::operator()(const Exponential& lhs, const Exponential& rhs) const {
    return lhs * rhs;
}

Multiply operator*(const Expression& lhs, const Expression& rhs) {
    return {lhs, rhs};
}