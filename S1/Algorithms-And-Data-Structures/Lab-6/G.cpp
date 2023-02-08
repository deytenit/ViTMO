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

struct SegmentTree {
    struct Node {
        long long value = 0;
        long long am = 0;
        int push = -1;
    };

    size_t size_ = 2;

    vector<Node> tree;

    SegmentTree(int n) {
        while (size_ < n) {
            size_ <<= 1;
        }

        tree.resize(size_ << 1, Node());

        build(1, 0, n);
    }

    long long get_sum(int v) {
        if (tree[v].push == -1) {
            return tree[v].value;
        } else {
            return tree[v].push * tree[v].am;
        }
    }

    void relax(int v) {
        if (v < size_) {
            tree[v].value = get_sum(v * 2) + get_sum(v * 2 + 1);
        }
    }

    void push(int v) {
        if (v < size_ && tree[v].push != -1) {
            tree[v * 2].push = tree[v * 2 + 1].push = tree[v].push;
            tree[v].push = -1;
            relax(v);
        }
    }

    void build(int v, int l, int r) {
        if (r - l == 1) {
            tree[v].am = 1;
            return;
        }
        push(v);

        int m = (l + r) / 2;

        build(v * 2, l, m);
        build(v * 2 + 1, m, r);

        tree[v].am = tree[v * 2].am + tree[v * 2 + 1].am;
    }

    void change(int v, int l, int r, int pos, ll x) {
        if (r - l == 1 && pos == l) {
            tree[v].value = max(0LL, tree[v].value + x);
            return;
        }

        push(v);

        int m = (l + r) / 2;

        if (pos < m) {
            change(v * 2, l, m, pos, x);
        } else {
            change(v * 2 + 1, m, r, pos, x);
        }
        relax(v);
    }

    ll get_f(int v, int l, int r, int L, int R) {
        if (R <= l || r <= L) {
            return 0;
        }

        if (L <= l && r <= R) {
            return get_sum(v);
        }

        push(v);

        int m = (l + r) / 2;

        return get_f(v * 2, l, m, L, R) + get_f(v * 2 + 1, m, r, L, R);
    }
};

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n, m;
    cin >> n >> m;

    SegmentTree tree(n);

    for (int i = 0; i < m; ++i) {
        char c;
        int k, x;

        cin >> c >> k >> x;

        switch (c) {
            case '+':
                tree.change(1, 0, n, k - 1, x);
                break;
            case '-':
                tree.change(1, 0, n, k - 1, -x);
                break;
            case '?':
                cout << tree.get_f(1, 0, n, k - 1, x) << '\n';
                break;
            default:
                break;
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
