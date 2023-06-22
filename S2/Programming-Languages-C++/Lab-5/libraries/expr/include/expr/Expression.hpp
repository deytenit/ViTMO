#ifndef EXPR_EXPRESSION_HPP
#define EXPR_EXPRESSION_HPP

#include <map>
#include <memory>

#include "type/Exponential.hpp"

class Expression {
public:
    Expression()                                   = default;
    Expression(const Expression& other)            = default;
    Expression(Expression&& other)                 = default;
    Expression& operator=(const Expression& other) = default;
    Expression& operator=(Expression&& other)      = default;

    [[nodiscard]] virtual Expression* clone() const = 0;

    [[nodiscard]] virtual Exponential eval(const std::map<std::string, Exponential>& mapping) const = 0;

    [[nodiscard]] virtual std::string str() const = 0;

    virtual ~Expression() = default;
};

class UnaryExpression: public Expression {
public:
    [[nodiscard]] Exponential eval(const std::map<std::string, Exponential>& mapping) const override;
    [[nodiscard]] std::string str() const override;

protected:
    UnaryExpression(const Expression& value);

    virtual Exponential operator()(const Exponential& lhs) const = 0;

    virtual char operation() const = 0;

    std::shared_ptr<Expression> m_value;
};

class BinaryExpression: public Expression {
public:
    [[nodiscard]] Exponential eval(const std::map<std::string, Exponential>& mapping) const override;

    [[nodiscard]] std::string str() const override;

protected:
    BinaryExpression(const Expression& lhs, const Expression& rhs);

    virtual Exponential operator()(const Exponential& lhs, const Exponential& rhs) const = 0;

    virtual char operation() const = 0;

    std::shared_ptr<Expression> m_lhs;
    std::shared_ptr<Expression> m_rhs;
};

std::ostream& operator<<(std::ostream& out, const Expression& object);

#endif