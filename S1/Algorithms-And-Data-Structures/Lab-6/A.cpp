#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
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
    long long key = 0;
    long long height = 0;
    Node* left = nullptr;
    Node* right = nullptr;

    Node() = default;
    Node(long long key) : key(key) {}
};

long long depth = 0;

void height_map(Node* node) {
    if (node->left) {
        height_map(node->left);
    }

    if (node->right) {
        height_map(node->right);
    }

    if (node->left && node->right) {
        node->height = max(node->right->height, node->left->height) + 1;
    } else if (node->left) {
        node->height = node->left->height + 1;
    } else if (node->right) {
        node->height = node->right->height + 1;
    } else {
        node->height = 1;
    }
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

        nodes[i]->key = x;

        if (l > 0) {
            nodes[i]->left = nodes[l - 1];
        }
        if (r > 0) {
            nodes[i]->right = nodes[r - 1];
        }
    }

    height_map(nodes.front());

    for (int i = 0; i < n; ++i) {
        if (nodes[i]->left && nodes[i]->right) {
            cout << nodes[i]->right->height - nodes[i]->left->height << '\n';
        } else if (nodes[i]->left) {
            cout << -nodes[i]->left->height << '\n';
        } else if (nodes[i]->right) {
            cout << nodes[i]->right->height << '\n';
        } else {
            cout << 0 << '\n';
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
