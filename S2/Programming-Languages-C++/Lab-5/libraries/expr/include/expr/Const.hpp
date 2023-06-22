#ifndef EXPR_CONST_HPP
#define EXPR_CONST_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Const final: public Expression {
public:
    Const(Exponential value);

    [[nodiscard]] Expression* clone() const override;

    [[nodiscard]] Exponential eval(const std::map<std::string, Exponential>&) const override;

    [[nodiscard]] std::string str() const override;

private:
    Exponential m_value;
};

#endif