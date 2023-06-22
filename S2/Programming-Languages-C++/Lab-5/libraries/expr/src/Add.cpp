#include "expr/Add.hpp"

Add::Add(const Expression& lhs, const Expression& rhs) : BinaryExpression(lhs, rhs) {}

Expression* Add::clone() const {
    return new Add(*this);
}

char Add::operation() const {
    return OPERATION;
}

Exponential Add::operator()(const Exponential& lhs, const Exponential& rhs) const {
    return lhs + rhs;
}

Add operator+(const Expression& lhs, const Expression& rhs) {
    return {lhs, rhs};
}