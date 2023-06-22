#ifndef EXPR_SUBTRACT_HPP
#define EXPR_SUBTRACT_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Subtract final: public BinaryExpression {
public:
    static constexpr char OPERATION = '-';

    Subtract(const Expression& lhs, const Expression& rhs);

    [[nodiscard]] Expression* clone() const override;

private:
    [[nodiscard]] char operation() const override;

    Exponential operator()(const Exponential& lhs, const Exponential& rhs) const override;
};

Subtract operator-(const Expression& lhs, const Expression& rhs);

#endif