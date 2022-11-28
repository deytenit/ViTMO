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

template <class T, class K>
struct SearchTreeMap {
    struct Node {
        T key;
        K value;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(T key, K value) : key(key), value(value) {}
    };

    Node* root = nullptr;

    void insert(T key, K value) {
        if (!root) {
            root = new Node(key, value);
            return;
        }

        insert(key, value, root);
    }

    void insert(T key, K value, Node* curr) {
        if (key == curr->key) {
            return;
        }

        if (key < curr->key) {
            if (curr->left) {
                insert(key, value, curr->left);
            } else {
                curr->left = new Node(key, value);
            }
        } else {
            if (curr->right) {
                insert(key, value, curr->right);
            } else {
                curr->right = new Node(key, value);
            }
        }
    }

    void remove(T key) {
        if (!root) {
            return;
        }

        remove(key, root, nullptr);
    }

    void remove(T key, Node* curr, Node* prev) {
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

    bool exists(T key) {
        return root ? exists(key, root) : false;
    }

    bool exists(T key, Node* curr) {
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

    Node* find(T key) {
        return root ? find(key, root) : nullptr;
    }

    Node* find(T key, Node* curr) {
        if (key == curr->key) {
            return curr;
        }

        if (key < curr->key && curr->left) {
            return find(key, curr->left);
        }

        if (key > curr->key && curr->right) {
            return find(key, curr->right);
        }

        return nullptr;
    }

    Node* next(T key) {
        if (!root) {
            return nullptr;
        }

        return next(key, root);
    }

    Node* next(T key, Node* curr) {
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

    Node* prev(T key) {
        if (!root) {
            return nullptr;
        }

        return prev(key, root);
    }

    Node* prev(T key, Node* curr) {
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

    void for_each(function<void(Node*)> func) {
        if (!root) {
            return;
        }

        for_each(func, root);
    }

    void for_each(function<void(Node*)> func, Node* curr) {
        if (curr->left) {
            for_each(func, curr->left);
        }

        func(curr);

        if (curr->right) {
            for_each(func, curr->right);
        }
    }
};

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    SearchTreeMap<string, array<bool, 3>> st;

    int n;
    cin >> n;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < n; ++j) {
            string s;
            cin >> s;
            SearchTreeMap<string, array<bool, 3>>::Node* query = st.find(s);
            if (!query) {
                array<bool, 3> arr{false};
                arr[i] = true;
                st.insert(s, arr);
            } else {
                query->value[i] = true;
            }
        }
    }

    array<int, 3> res{0};

    st.for_each([&res](SearchTreeMap<string, array<bool, 3>>::Node* curr) {
        int amo = 0;

        for (int i = 0; i < 3; ++i) {
            if (curr->value[i]) {
                ++amo;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (curr->value[i]) {
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