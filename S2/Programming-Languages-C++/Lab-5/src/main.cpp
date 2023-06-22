#include <expr/expressions.hpp>
#include <iomanip>
#include <iostream>

int main() {
    const Exponential a(6523737337250884473, 10);
    const Exponential b(-6519742445237669332, 10);

    std::cout << a + b << '\n';

    const Add expr_a(Multiply(Const(2), Variable("x")), Const(1));
    const Add expr_b = Const(2) * Variable("x") + Const(1);

    const Exponential result_a = expr_a.eval({{"x", Exponential(100)}, {"y", Exponential(42)}});
    const Exponential result_b = expr_b.eval({{"x", Exponential(100)}, {"y", Exponential(42)}});

    std::cout << result_a << ' ' << result_b << '\n';

    return 0;
}