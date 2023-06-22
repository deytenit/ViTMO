#ifndef EXPR_ADD_HPP
#define EXPR_ADD_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Add final: public BinaryExpression {
public:
    static constexpr char OPERATION = '+';

    Add(const Expression& lhs, const Expression& rhs);

    [[nodiscard]] Expression* clone() const override;

private:
    [[nodiscard]] char operation() const override;

    Exponential operator()(const Exponential& lhs, const Exponential& rhs) const override;
};

Add operator+(const Expression& lhs, const Expression& rhs);

#endif