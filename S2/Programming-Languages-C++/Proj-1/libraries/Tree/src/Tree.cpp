#include "tree/Tree.hpp"

Treap::Node::~Node() {
    delete lnode;
    delete rnode;
}

bool Treap::contains(const int value) const noexcept {
    return find(root_, value);
}

bool Treap::insert(const int value) {
    if (contains(value)) {
        return false;
    }

    Node* node = make_node(value);

    insert(root_, node);

    return true;
}

bool Treap::remove(const int value) {
    if (!contains(value)) {
        return false;
    }

    remove(root_, value);

    return true;
}

std::size_t Treap::size() const noexcept {
    return size(root_);
}

bool Treap::empty() const noexcept {
    return size(root_) == 0;
}

void Treap::clear() noexcept {
    delete root_;
    root_ = nullptr;
}

std::vector<int> Treap::values() const noexcept {
    std::vector<int> result;

    values(root_, result);

    return result;
}

Treap::~Treap() {
    clear();
}

Treap::Node* Treap::make_node(const int value) {
    return new Node{value, distribution(random)};
}

void Treap::remove_node(Node*& node) {
    node->lnode = node->rnode = nullptr;
    delete node;
}

std::size_t Treap::size(const Node* subroot) {
    return subroot ? subroot->size : 0;
}

void Treap::update_size(Node*& subroot) {
    if (subroot) {
        subroot->size = 1 + size(subroot->lnode) + size(subroot->rnode);
    }
}

const Treap::Node* Treap::find(const Node* current, const int value) {
    if (!current) {
        return nullptr;
    }

    if (current->value == value) {
        return current;
    }

    return find(value < current->value ? current->lnode : current->rnode, value);
}

void Treap::insert(Node*& current, Node* node) {
    if (!current) {
        current = node;
        return;
    }

    if (node->priority > current->priority) {
        split(current, node->value, node->lnode, node->rnode);
        current = node;
    } else {
        insert(current->value <= node->value ? current->rnode : current->lnode, node);
    }

    update_size(current);
}

void Treap::remove(Node*& current, const int value) {
    if (current->value == value) {
        Node* temp = current;
        merge(current, current->lnode, current->rnode);
        remove_node(temp);
    } else {
        remove(value < current->value ? current->lnode : current->rnode, value);
    }

    update_size(current);
}

void Treap::split(Node* subroot, const int value, Node*& lnode, Node*& rnode) {
    if (!subroot) {
        lnode = rnode = nullptr;
        return;
    }

    if (subroot->value <= value) {
        split(subroot->rnode, value, subroot->rnode, rnode);
        lnode = subroot;
    } else {
        split(subroot->lnode, value, lnode, subroot->lnode);
        rnode = subroot;
    }

    update_size(subroot);
}

void Treap::merge(Node*& subroot, Node* lnode, Node* rnode) {
    if (!lnode || !rnode) {
        subroot = lnode ? lnode : rnode;
        return;
    }

    if (lnode->priority > rnode->priority) {
        merge(lnode->rnode, lnode->rnode, rnode);
        subroot = lnode;
    } else {
        merge(rnode->lnode, lnode, rnode->lnode);
        subroot = rnode;
    }

    update_size(subroot);
}

void Treap::values(const Node* current, std::vector<int>& result) {
    if (!current) {
        return;
    }

    if (current->lnode) {
        values(current->lnode, result);
    }

    result.push_back(current->value);

    if (current->rnode) {
        values(current->rnode, result);
    }
}
