#ifndef TREE_HPP
#define TREE_HPP

#include <cstddef>
#include <random>
#include <vector>

class Treap {
public:
    struct Node {
        int value{};
        int priority{};
        Node* lnode = nullptr;
        Node* rnode = nullptr;

        std::size_t size = 1;

        ~Node();
    };

private:
    std::default_random_engine random{std::random_device{}()};
    std::uniform_int_distribution<> distribution{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
    Node* root_ = nullptr;

public:
    [[nodiscard]] bool contains(int value) const noexcept;
    bool insert(int value);
    bool remove(int value);

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    void clear() noexcept;

    [[nodiscard]] std::vector<int> values() const noexcept;

    ~Treap();

private:
    [[nodiscard]] Node* make_node(int value);
    static void remove_node(Node*& node);

    [[nodiscard]] static std::size_t size(const Node* subroot);
    static void update_size(Node*& subroot);

    [[nodiscard]] static const Node* find(const Node* current, int value);

    static void insert(Node*& current, Node* node);
    static void remove(Node*& current, int value);

    static void split(Node* subroot, int value, Node*& lnode, Node*& rnode);
    static void merge(Node*& subroot, Node* lnode, Node* rnode);

    static void values(const Node* current, std::vector<int>& result);
};

#endif
