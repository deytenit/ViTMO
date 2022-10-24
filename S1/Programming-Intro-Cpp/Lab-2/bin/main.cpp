
#include <lib/number.h>

#include <iostream>

int main() {
    uint2022_t value(std::string("4052723123306066839824984"));
    uint2022_t value2(std::string("98765432109876543210"));
    uint2022_t v1(uint2022_t::NT{0, 10});
    uint2022_t v2(3);

    uint2022_t alpha = v1 * v2;
    uint2022_t aboba = v1 / v2;

    std::cout << value << ' ' << value2 << std::endl;

    return 0;
}
