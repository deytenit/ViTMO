#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define sqr(a) ((a) * (a))
#define vout(a) cout << #a << ": " << (a) << '\n'

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;

struct Node {
    long long key_ = 0;
    long long height_ = 0;

    size_t num_ = 0;

    Node* l_ = nullptr;
    Node* r_ = nullptr;

    Node() = default;
    Node(long long key) : key_(key) {}
};

void height_map(Node* node) {
    if (node->l_) {
        height_map(node->l_);
    }

    if (node->r_) {
        height_map(node->r_);
    }

    if (node->l_ && node->r_) {
        node->height_ = max(node->r_->height_, node->l_->height_) + 1;
    } else if (node->l_) {
        node->height_ = node->l_->height_ + 1;
    } else if (node->r_) {
        node->height_ = node->r_->height_ + 1;
    } else {
        node->height_ = 1;
    }
}

Node* little_turn_left(Node* node) {
    Node* a = node;
    Node* b = a->r_;

    a->r_ = b->l_;
    b->l_ = a;

    return b;
}

Node* big_turn_left(Node* node) {
    Node* a = node;
    Node* b = a->r_;
    Node* c = b->l_;
    a->r_ = c->l_;
    b->l_ = c->r_;

    c->l_ = a;
    c->r_ = b;

    return c;
}

long long height(Node* node) {
    return node ? node->height_ : 0;
}

long long nbbal(Node* node) {
    return node ? height(node->r_) - height(node->l_) : 0;
}

vector<Node*> to_vector(Node* node) {
    vector<Node*> result;

    size_t num = 1;

    queue<Node*> q;
    q.push(node);

    while (!q.empty()) {
        Node* tmp = q.front();
        q.pop();

        tmp->num_ = num++;
        result.push_back(tmp);

        if (tmp->l_) {
            q.push(tmp->l_);
        }

        if (tmp->r_) {
            q.push(tmp->r_);
        }
    }

    return result;
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n;
    cin >> n;

    std::vector<Node*> nodes(n);

    for (int i = 0; i < n; ++i) {
        nodes[i] = new Node();
    }

    for (int i = 0; i < n; ++i) {
        int x, l, r;
        cin >> x >> l >> r;

        nodes[i]->key_ = x;

        if (l > 0) {
            nodes[i]->l_ = nodes[l - 1];
        }
        if (r > 0) {
            nodes[i]->r_ = nodes[r - 1];
        }
    }

    Node* root = nodes.front();

    height_map(root);

    if (nbbal(root->r_) == -1) {
        root = big_turn_left(nodes.front());
    } else {
        root = little_turn_left(nodes.front());
    }

    vector<Node*> output = to_vector(root);

    cout << output.size() << '\n';

    for (size_t i = 0; i < output.size(); ++i) {
        cout << output[i]->key_ << ' ';
        cout << (output[i]->l_ ? output[i]->l_->num_ : 0) << ' ';
        cout << (output[i]->r_ ? output[i]->r_->num_ : 0) << '\n';
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
