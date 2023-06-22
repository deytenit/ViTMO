#ifndef EXPR_NEGATE_HPP
#define EXPR_NEGATE_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Negate final: public UnaryExpression {
public:
    static constexpr char OPERATION = '-';

    Negate(const Expression& value) : UnaryExpression(value) {}

    [[nodiscard]] Expression* clone() const override { return new Negate(*this); }

private:
    [[nodiscard]] char operation() const override { return OPERATION; }

    Exponential operator()(const Exponential& value) const override { return -value; }
};

#endif