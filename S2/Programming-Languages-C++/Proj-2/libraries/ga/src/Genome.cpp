#include "ga/Genome.hpp"

#include <algorithm>
#include <execution>
#include <ranges>
#include <stack>
#include <unordered_map>
#include <unordered_set>

namespace {
class Graph {
public:
    class Node {
    public:
        std::string name;
        std::vector<Node*> out;
        std::vector<Node*> in;

        explicit Node(std::string_view name);

        static void add_vertex(Node& from, Node& to);
    };

public:
    void add_vertex(std::string_view from, std::string_view to);

    std::vector<std::string_view> find_path();

private:
    std::vector<std::unique_ptr<Node>> m_nodes;
    std::unordered_map<std::string_view, Node*> m_latest;
};

Graph::Node::Node(std::string_view name) : name(name) {}

void Graph::Node::add_vertex(Node& from, Node& to) {
    from.out.push_back(&to);
    to.in.push_back(&from);
}

void Graph::add_vertex(std::string_view from, std::string_view to) {
    Node* from_node;
    Node* to_node;

    if (!m_latest.count(from)) {
        from_node = new Node(from);
        m_nodes.emplace_back(from_node);
        m_latest.emplace(from, from_node);
    } else {
        from_node = m_latest[from];
    }

    if (from == to || !m_latest.count(to)) {
        to_node = new Node(to);
        m_nodes.emplace_back(to_node);
        m_latest.insert_or_assign(to, to_node);
    } else {
        to_node = m_latest[to];
    }

    Node::add_vertex(*from_node, *to_node);
}

std::vector<std::string_view> Graph::find_path() {
    Node* begin_node = std::find_if(std::execution::par, m_nodes.begin(), m_nodes.end(), [](const auto& node) {
                           return node->out.size() - node->in.size() == 1;
                       })->get();

    std::vector<Node*> path;
    std::stack<Node*, std::vector<Node*>> stack;

    stack.push(begin_node);

    while (!stack.empty()) {
        Node* node = stack.top();
        if (!node->out.empty()) {
            stack.push(node->out.back());
            node->out.pop_back();
            continue;
        }

        path.emplace_back(node);
        stack.pop();
    }

    std::vector<std::string_view> result;

    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        result.emplace_back((*it)->name);
    }

    return result;
}

std::string assemble_string(std::vector<std::string_view>&& data) {
    std::string result{data.front()};

    for (size_t i = 1; i < data.size(); ++i) {
        result += data[i].back();
    }

    return result;
}
}  // namespace

namespace genome {
std::string assembly(const size_t k, const std::vector<std::string>& reads) {
    if (k == 0 || reads.empty()) {
        return "";
    }

    Graph graph;

    for (const std::string_view read : reads) {
        for (size_t i = k + 1; i <= read.size(); ++i) {
            graph.add_vertex(read.substr(i - k - 1, k), read.substr(i - k, k));
        }
    }

    return assemble_string(graph.find_path());
}
}  // namespace genome
