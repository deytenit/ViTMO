#ifndef EXPR_MULTIPLY_HPP
#define EXPR_MULTIPLY_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Multiply final: public BinaryExpression {
public:
    static constexpr char OPERATION = '*';

    Multiply(const Expression& lhs, const Expression& rhs);

    [[nodiscard]] Expression* clone() const override;

protected:
    [[nodiscard]] char operation() const override;

    Exponential operator()(const Exponential& lhs, const Exponential& rhs) const override;
};

Multiply operator*(const Expression& lhs, const Expression& rhs);

#endif