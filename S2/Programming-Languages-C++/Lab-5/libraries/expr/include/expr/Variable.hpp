#ifndef EXPR_VARIABLE_HPP
#define EXPR_VARIABLE_HPP

#include "Expression.hpp"
#include "type/Exponential.hpp"

class Variable: public Expression {
public:
    Variable(std::string name);

    [[nodiscard]] Expression* clone() const override;

    [[nodiscard]] Exponential eval(const std::map<std::string, Exponential>& mapping) const override;

    [[nodiscard]] std::string str() const override;

private:
    std::string m_name;
};

#endif