#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

#define ff first
#define ss second
#define sqr(a) ((a) * (a))
#define vout(a) cout << #a << ": " << (a) << '\n'

using ll = long long;
using ld = long double;

constexpr int INF = 0x7fffffff;
constexpr ll MOD = 1e9 + 7;
constexpr ld EPS = 1e-8;
constexpr ld PI = 3.14159265358979323;
constexpr int MAXN = 1e5 + 1000;

int a[MAXN];

struct Node {
    int priority;
    int cnt;
    int mn;
    bool rev;
    Node *l, *r;
    Node() : priority{0}, cnt{1}, mn{0}, rev{false}, l{nullptr}, r{nullptr} {}
    Node(int priority) : priority{priority}, cnt{1}, mn{priority}, rev{false}, l{nullptr}, r{nullptr} {}
};

int cnt(Node *node) {
    return node ? node->cnt : 0;
}

int mn(Node *node) {
    return node ? node->mn : INF;
}

void upd(Node *node) {
    if (node) {
        node->cnt = cnt(node->l) + cnt(node->r) + 1;
        node->mn = min({node->priority, mn(node->l), mn(node->r)});
    }
}

void push(Node *node) {
    if (node && node->rev) {
        node->rev = false;
        swap(node->l, node->r);
        if (node->l) {
            node->l->rev ^= true;
        }
        if (node->r) {
            node->r->rev ^= true;
        }
    }
}

void merge(Node *&node, Node *l, Node *r) {
    push(l);
    push(r);
    if (!l || !r) {
        node = l ? l : r;
    } else if (l->priority > r->priority) {
        merge(l->r, l->r, r);
        node = l;
    } else {
        merge(r->l, l, r->l);
        node = r;
    }

    upd(node);
}

void split(Node *node, Node *&l, Node *&r, int key, int add = 0) {
    if (!node) {
        l = nullptr;
        r = nullptr;
        return;
    }

    push(node);

    int idx = add + cnt(node->l);
    if (key <= idx) {
        split(node->l, l, node->l, key, add);
        r = node;
    } else {
        split(node->r, node->r, r, key, add + cnt(node->l) + 1);
        l = node;
    }
    upd(node);
}

void insert(Node *&node, int pos, int val) {
    Node *node1, *node2;
    split(node, node1, node2, pos);
    merge(node1, node1, new Node{val});
    merge(node, node1, node2);
}

int min(Node *node, int l, int r) {
    Node *node1, *node2, *node3;
    split(node, node2, node3, r + 1);
    split(node2, node1, node2, l);

    int ans = mn(node2);

    merge(node, node1, node2);
    merge(node, node, node3);

    return ans;
}

void reverse(Node *node, int l, int r) {
    Node *node1, *node2, *node3;
    split(node, node1, node2, l);
    split(node2, node2, node3, r - l + 1);
    node2->rev ^= true;
    merge(node, node1, node2);
    merge(node, node, node3);
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    Node *rt = nullptr;

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        insert(rt, i, x);
    }

    while (m--) {
        int cmd, l, r;
        cin >> cmd >> l >> r;
        --l, --r;
        switch (cmd) {
            case 1: {
                reverse(rt, l, r);
                break;
            }
            case 2: {
                cout << min(rt, l, r) << '\n';
                break;
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}