#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <iostream>
#include <random>
#include <sstream>

#include "expr/type/Exponential.hpp"

TEST_CASE("Crafting", "[exponential]") {
    SECTION("default") {
        Exponential a;
        Exponential b = 0;

        REQUIRE(a.significand() == 0);
        REQUIRE(a == b);
    }
    SECTION("limits") {
        Exponential a = std::numeric_limits<Exponential::data_type>::min();
        Exponential b = std::numeric_limits<Exponential::data_type>::max();
        REQUIRE(a + b == Exponential(-1));
    }
    SECTION("assignment (rvalue + lvalue)") {
        Exponential a(3);
        Exponential b = a;
        a             = 5;

        REQUIRE(b == 3);
        REQUIRE(b != a);
        REQUIRE(a == 5);
    }
    SECTION("assignment (self + return)") {
        Exponential a(228);
        Exponential b(1337);
        (a = b) = a;

        REQUIRE(a == 1337);
        REQUIRE(b == 1337);
    }
    SECTION("significant + exponent") {
        Exponential a(0, 0);
        Exponential b(-7, 13);

        REQUIRE(a.significand() == 0);
        REQUIRE(a.exponent() == 0);

        REQUIRE(b.significand() == -7);
        REQUIRE(b.exponent() == 13);
    }
}

TEST_CASE("Returns", "[exponential]") {
    SECTION("to string") {
        Exponential a(-7, 13);
        Exponential b(7, -13);

        REQUIRE(a.str() == "-7e+13");

        REQUIRE(b.str() == "7e-13");
    }
    SECTION("to double") {
        Exponential a(7);
        Exponential b(13, -3);

        REQUIRE_THAT(static_cast<double>(a), Catch::Matchers::WithinRel(7.0));
        REQUIRE_THAT(static_cast<double>(b), Catch::Matchers::WithinRel(0.013));
    }
    SECTION("to stream") {
        Exponential a(-7, 13);
        Exponential b(7, -13);

        std::stringstream out;

        out << a << ' ' << b << '\n';

        REQUIRE(out.str() == "-7e+13 7e-13\n");
    }
}

TEST_CASE("Comparsions", "[exponential]") {
    SECTION("equality") {
        Exponential a(7);
        Exponential b(7);
        Exponential c(13);

        REQUIRE(a == b);
        REQUIRE_FALSE(a == c);
    }
    SECTION("inequality") {
        Exponential a(7);
        Exponential b(7);
        Exponential c(13);

        REQUIRE(a != c);
        REQUIRE_FALSE(a != b);
    }
    SECTION("everything") {
        Exponential a(7);
        Exponential b(7);
        Exponential c(13);

        REQUIRE(a == b);
        REQUIRE(a != c);
        REQUIRE(a < c);
        REQUIRE(a <= a);
        REQUIRE(a <= b);
        REQUIRE(a <= c);
        REQUIRE(c > a);
        REQUIRE(c >= a);
    }
}

TEST_CASE("Operations (pre)", "[exponential]") {
    SECTION("Add") {
        Exponential a(2);
        Exponential b(-2);

        REQUIRE(2 + a == 4);
        REQUIRE(b + 4 == 2);
    }
    SECTION("Subtract") {
        Exponential a(2);
        Exponential b(-2);

        REQUIRE(12 - a == 10);
        REQUIRE(b - 4 == -6);
    }
    SECTION("Multiply") {
        Exponential a(13);
        Exponential b(7);

        REQUIRE(4 * a == 52);
        REQUIRE(b * 8 == 56);
    }
    SECTION("Divide") {
        Exponential a(4);
        Exponential b(56);

        REQUIRE(52 / a == 13);
        REQUIRE(b / 8 == 7);
    }
    SECTION("Inf") {
        Exponential a(std::numeric_limits<Exponential::data_type>::max() - 1,
                      std::numeric_limits<Exponential::data_type>::max() - 1);
        Exponential b(a);

        REQUIRE((a + b).isinf());
        REQUIRE((a * b).isinf());
        REQUIRE(((-a) - b).isinf());
        REQUIRE(((-a) * b).isinf());
        REQUIRE((a / 0).isinf());
        REQUIRE((a + Exponential::INF).isinf());
        REQUIRE(a / Exponential::INF == 0);
    }
    SECTION("Nan") {
        REQUIRE((-Exponential::INF + Exponential::INF).isnan());
        REQUIRE((Exponential::INF / Exponential::INF).isnan());
        REQUIRE((Exponential::INF * std::numeric_limits<double>::quiet_NaN()).isnan());
    }
}

TEST_CASE("Operations (random)", "[exponential]") {
    constexpr int COUNT = 5000;

    auto real_a = Catch::Generators::random(1e-18, 1e18);
    auto real_b = Catch::Generators::random(1e-18, 1e18);

    for (int i = 0; i < COUNT; ++i) {
        REQUIRE_THAT(Exponential(real_a.get()) + Exponential(real_b.get()),
                     Catch::Matchers::WithinRel(real_a.get() + real_b.get()));
        REQUIRE_THAT(Exponential(real_a.get()) - Exponential(real_b.get()),
                     Catch::Matchers::WithinRel(real_a.get() - real_b.get()));
        REQUIRE_THAT(Exponential(real_a.get()) * Exponential(real_b.get()),
                     Catch::Matchers::WithinRel(real_a.get() * real_b.get()));
        REQUIRE_THAT(Exponential(real_a.get()) / Exponential(real_b.get()),
                     Catch::Matchers::WithinRel(real_a.get() / real_b.get()));

        real_a.next();
        real_b.next();
    }
}