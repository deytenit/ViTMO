#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
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

struct SearchTree {
    struct Node {
        long long key = 0;
        Node* left = nullptr;
        Node* right = nullptr;

        Node() = default;
        Node(long long key) : key(key) {}
    };

    Node* root = nullptr;

    void insert(long long key) {
        if (!root) {
            root = new Node(key);
            return;
        }

        insert(key, root);
    }

    void insert(long long key, Node* curr) {
        if (key == curr->key) {
            return;
        }

        if (key < curr->key) {
            if (curr->left) {
                insert(key, curr->left);
            } else {
                curr->left = new Node(key);
            }
        } else {
            if (curr->right) {
                insert(key, curr->right);
            } else {
                curr->right = new Node(key);
            }
        }
    }

    void remove(long long key) {
        if (!root) {
            return;
        }

        remove(key, root, nullptr);
    }

    void remove(long long key, Node* curr, Node* prev) {
        if (key == curr->key) {
            if (!curr->left && !curr->right) {
                if (prev) {
                    if (key < prev->key) {
                        prev->left = nullptr;
                    } else {
                        prev->right = nullptr;
                    }
                } else {
                    root = nullptr;
                }

                delete curr;
            } else if (curr->left && curr->right) {
                Node* tmp = next(curr->key, curr);

                curr->key = tmp->key;

                remove(curr->key, curr->right, curr);
            } else {
                if (prev) {
                    if (key < prev->key) {
                        prev->left = curr->left ? curr->left : curr->right;
                    } else {
                        prev->right = curr->left ? curr->left : curr->right;
                    }
                } else {
                    root = curr->left ? curr->left : curr->right;
                }

                delete curr;
            }
            return;
        }

        if (key < curr->key && curr->left) {
            remove(key, curr->left, curr);
        } else if (key > curr->key && curr->right) {
            remove(key, curr->right, curr);
        }
    }

    bool exists(long long key) {
        return root ? exists(key, root) : false;
    }

    bool exists(long long key, Node* curr) {
        if (key == curr->key) {
            return true;
        }

        if (key < curr->key && curr->left) {
            return exists(key, curr->left);
        }

        if (key > curr->key && curr->right) {
            return exists(key, curr->right);
        }

        return false;
    }

    Node* next(long long key) {
        if (!root) {
            return nullptr;
        }

        return next(key, root);
    }

    Node* next(long long key, Node* curr) {
        Node* tmp = nullptr;

        if (key < curr->key && curr->left) {
            tmp = next(key, curr->left);
        }

        if (key >= curr->key && curr->right) {
            tmp = next(key, curr->right);
        }

        if (tmp && tmp->key > key) {
            return tmp;
        }

        if (curr->key > key) {
            return curr;
        }

        return nullptr;
    }

    Node* prev(long long key) {
        if (!root) {
            return nullptr;
        }

        return prev(key, root);
    }

    Node* prev(long long key, Node* curr) {
        Node* tmp = nullptr;

        if (key <= curr->key && curr->left) {
            tmp = prev(key, curr->left);
        }

        if (key > curr->key && curr->right) {
            tmp = prev(key, curr->right);
        }

        if (tmp && tmp->key < key) {
            return tmp;
        }

        if (curr->key < key) {
            return curr;
        }

        return nullptr;
    }
};

int idx = 0;

void to_vector(SearchTree::Node* node, vector<int>& out) {
    if (node->left) {
        to_vector(node->left, out);
    }

    out[idx++] = node->key;

    if (node->right) {
        to_vector(node->right, out);
    }
}

bool approve(SearchTree::Node* node, size_t size) {
    vector<int> vals(size, 0);
    to_vector(node, vals);

    for (int i = 1; i < vals.size(); ++i) {
        if (vals[i] <= vals[i - 1]) {
            return false;
        }
    }

    return true;
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

    std::vector<SearchTree::Node*> nodes(n);

    for (int i = 0; i < n; ++i) {
        nodes[i] = new SearchTree::Node();
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

    cout << (n == 0 || approve(nodes[0], n) ? "YES\n" : "NO\n");

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}