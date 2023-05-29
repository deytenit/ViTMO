#include <iostream>

#include "tree/Tree.hpp"

int main() {
    Treap treap;

    treap.insert(2);
    treap.insert(3);

    std::cout << treap.contains(2) << std::endl;
    std::cout << treap.contains(3) << std::endl;

    treap.remove(3);

    std::cout << treap.contains(2) << std::endl;
    std::cout << treap.contains(3) << std::endl;

    return 0;
}
