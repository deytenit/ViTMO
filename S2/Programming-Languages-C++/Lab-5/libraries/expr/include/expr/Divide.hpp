#ifndef EXPR_DIVIDE_HPP
#define EXPR_DIVIDE_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Divide final: public BinaryExpression {
public:
    static constexpr char OPERATION = '/';

    Divide(const Expression& lhs, const Expression& rhs);

    [[nodiscard]] Expression* clone() const override;

private:
    [[nodiscard]] char operation() const override;

    Exponential operator()(const Exponential& lhs, const Exponential& rhs) const override;
};

Divide operator/(const Expression& lhs, const Expression& rhs);

#endif