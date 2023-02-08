#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <functional>
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

template <class T, class U>
struct AVLMap {
    struct Node {
        T key_;
        U value_;

        long long height_ = 1;

        Node* l_ = nullptr;
        Node* r_ = nullptr;

        Node(T key, U value) : key_(key), value_(value){};
    };

    Node* root_ = nullptr;
    size_t size_ = 0;

    static Node* right_turnaround(Node* a) {
        Node* b = a->l_;
        Node* c = b->r_;

        b->r_ = a;
        a->l_ = c;

        a->height_ = max(height(a->l_), height(a->r_)) + 1;
        b->height_ = max(height(b->l_), height(b->r_)) + 1;

        return b;
    }

    static Node* left_turnaround(Node* a) {
        Node* b = a->r_;
        Node* c = b->l_;

        b->l_ = a;
        a->r_ = c;

        a->height_ = max(height(a->l_), height(a->r_)) + 1;
        b->height_ = max(height(b->l_), height(b->r_)) + 1;

        return b;
    }

    void insert(T key, U value) {
        root_ = insert(key, value, root_);
    }

    Node* insert(T& key, U& value, Node* node) {
        if (!node) {
            ++size_;
            return new Node(key, value);
        }

        if (key < node->key_) {
            node->l_ = insert(key, value, node->l_);
        } else if (key > node->key_)
            node->r_ = insert(key, value, node->r_);
        else {
            return node;
        }

        node->height_ = max(height(node->l_), height(node->r_)) + 1;

        long long bbal = nbbal(node);

        if (bbal < -1) {
            if (nbbal(node->l_) <= 0) {
                return right_turnaround(node);
            } else {
                node->l_ = left_turnaround(node->l_);
                return right_turnaround(node);
            }
        }
        if (bbal > 1) {
            if (nbbal(node->r_) >= 0) {
                return left_turnaround(node);
            } else {
                node->r_ = right_turnaround(node->r_);
                return left_turnaround(node);
            }
        }
        return node;
    }

    void remove(T key) {
        root_ = remove(key, root_);
    }

    Node* remove(T& key, Node* node) {
        if (!node) {
            return node;
        }

        if (key < node->key_) {
            node->l_ = remove(key, node->l_);
        } else if (key > node->key_) {
            node->r_ = remove(key, node->r_);
        } else if (!node->l_ && !node->r_) {
            --size_;
            delete node;

            node = nullptr;
        } else if (node->l_) {
            Node* tmp = node->l_;
            while (tmp->r_) {
                tmp = tmp->r_;
            }
            node->key_ = tmp->key_;
            node->l_ = remove(tmp->key_, node->l_);
        } else {
            Node* tmp = node->r_;
            while (tmp->l_) {
                tmp = tmp->l_;
            }
            node->key_ = tmp->key_;
            node->r_ = remove(tmp->key_, node->r_);
        }

        if (!node) {
            return node;
        }

        node->height_ = max(height(node->l_), height(node->r_)) + 1;

        long long bbal = nbbal(node);

        if (bbal < -1) {
            if (nbbal(node->l_) <= 0) {
                return right_turnaround(node);
            } else {
                node->l_ = left_turnaround(node->l_);
                return right_turnaround(node);
            }
        }
        if (bbal > 1) {
            if (nbbal(node->r_) >= 0) {
                return left_turnaround(node);
            } else {
                node->r_ = right_turnaround(node->r_);
                return left_turnaround(node);
            }
        }

        return node;
    }

    Node* find(T key) {
        return find(key, root_);
    }

    Node* find(T key, Node* node) {
        if (!node) {
            return node;
        }

        if (node->key_ == key) {
            return node;
        }

        if (key < node->key_) {
            return find(key, node->l_);
        }

        if (key > node->key_) {
            return find(key, node->r_);
        }
    }

    void for_each(function<void(Node*)> func) {
        for_each(func, root_);
    }

    void for_each(function<void(Node*)> func, Node* node) {
        if (!node) {
            return;
        }

        if (node->l_) {
            for_each(func, node->l_);
        }

        func(node);

        if (node->r_) {
            for_each(func, node->r_);
        }
    }

    static long long height(Node* node) {
        return node ? node->height_ : 0;
    }

    static long long nbbal(Node* node) {
        return node ? height(node->r_) - height(node->l_) : 0;
    }
};

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    AVLMap<string, array<bool, 3>> st;

    int n;
    cin >> n;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < n; ++j) {
            string s;
            cin >> s;
            AVLMap<string, array<bool, 3>>::Node* query = st.find(s);
            if (!query) {
                array<bool, 3> arr{false};
                arr[i] = true;
                st.insert(s, arr);
            } else {
                query->value_[i] = true;
            }
        }
    }

    array<int, 3> res{0};

    st.for_each([&res](AVLMap<string, array<bool, 3>>::Node* curr) {
        int amo = 0;

        for (int i = 0; i < 3; ++i) {
            if (curr->value_[i]) {
                ++amo;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (curr->value_[i]) {
                res[i] = res[i] + (amo == 2 ? 1 : amo == 1 ? 3
                                                           : 0);
            }
        }
    });

    for (int i = 0; i < 3; ++i) {
        cout << res[i] << ' ';
    }
    cout << endl;

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
