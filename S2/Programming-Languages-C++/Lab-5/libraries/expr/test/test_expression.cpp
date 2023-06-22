#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>

#include "expr/expressions.hpp"

TEST_CASE("Crafting", "[expression]") {
    SECTION("const") {
        Const a(7);
        Const b(-13);

        REQUIRE(a.eval({}) == 7);
        REQUIRE(b.eval({}) == -13);
    }
    SECTION("variable") {
        Variable a("x");
        Variable b("y");

        REQUIRE(a.eval({{"x", 7}}) == 7);
        REQUIRE(b.eval({{"y", -13}}) == -13);
        REQUIRE(a.eval({{"y", -13}}).isnan());
    }
}

TEST_CASE("Returns", "[expression]") {
    SECTION("to string") {
        const Add a(Multiply(Const(2), Variable("x")), Negate(Const(1)));
        const Add b = Const(2) * Variable("x") + Negate(Const(1));

        REQUIRE(a.str() == "((2 * x) + -(1))");
        REQUIRE(b.str() == "((2 * x) + -(1))");
    }
    SECTION("to stream") {
        const Add a(Multiply(Const(2), Variable("x")), Negate(Const(1)));
        const Add b = Const(2) * Variable("x") + Negate(Const(1));

        std::stringstream out;

        out << a << ' ' << b;

        REQUIRE(out.str() == "((2 * x) + -(1)) ((2 * x) + -(1))");
    }
}

TEST_CASE("Operations (pre)", "[expression]") {
    SECTION("Add") {
        Add a(Const(2), Variable("x"));
        Add b = Const(2) + Variable("x");

        REQUIRE(a.eval({{"x", 2}}) == 4);
        REQUIRE(b.eval({{"x", 2}}) == 4);
    }
    SECTION("Subtract") {
        Subtract a(Const(12), Variable("x"));
        Subtract b = Variable("x") - Const(4);

        REQUIRE(a.eval({{"x", 2}}) == 10);
        REQUIRE(b.eval({{"x", -2}}) == -6);
    }
    SECTION("Multiply") {
        Multiply a(Const(4), Variable("x"));
        Multiply b = Variable("x") * Const(8);

        REQUIRE(a.eval({{"x", 13}}) == 52);
        REQUIRE(b.eval({{"x", 7}}) == 56);
    }
    SECTION("Divide") {
        Divide a(Const(52), Variable("x"));
        Divide b = Variable("x") / Const(8);

        REQUIRE(a.eval({{"x", 4}}) == 13);
        REQUIRE(b.eval({{"x", 56}}) == 7);
    }
    SECTION("Negate") {
        Const a(7);

        REQUIRE(Negate(a).eval({}) == -7);
    }
}

TEST_CASE("Operations (random)", "[expression]") {
    constexpr int COUNT = 228;

    auto numbers    = Catch::Generators::random(1e-18, 1e18);
    auto operations = Catch::Generators::random(0, 4);

    Exponential a(numbers.get());
    std::shared_ptr<Expression> b(Const(a).clone());
    for (int i = 0; i < COUNT; ++i) {
        numbers.next();
        operations.next();

        const Exponential tmp(numbers.get());
        switch (operations.get()) {
        case 0: {
            a += tmp;
            b = std::make_shared<Add>(*b + Const(tmp));
            break;
        }
        case 1: {
            a -= tmp;
            b = std::make_shared<Subtract>(*b - Const(tmp));
            break;
        }
        case 2: {
            a *= tmp;
            b = std::make_shared<Multiply>(*b * Const(tmp));
            break;
        }
        case 3: {
            a /= tmp;
            b = std::make_shared<Divide>(*b / Const(tmp));
            break;
        }
        case 4: {
            a = -a;
            b = std::make_shared<Negate>(Negate(*b));
            break;
        }
        default:
            break;
        }
    }

    REQUIRE(a == b->eval({}));
}