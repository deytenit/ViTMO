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
constexpr int MAXN = 5e5 + 1000;

struct br {
    int ln = 0;
    int lb = 0;
    int rb = 0;
};

int a[MAXN];
br t[MAXN << 2];

br fn(const br& x, const br& y, int l, int r) {
    int ln = max({x.ln, y.ln, x.rb + y.lb});

    int m = (l + r) >> 1;
    int lb = m - l + 1 != x.lb ? x.lb : x.lb + y.lb;
    int rb = r - m != y.rb ? y.rb : x.rb + y.rb;
    return {ln, lb, rb};
}

void build(int a[], int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl] == 0 ? br{1, 1, 1} : br{0, 0, 0};
    } else {
        int tm = (tl + tr) >> 1;
        build(a, v << 1, tl, tm);
        build(a, (v << 1) + 1, tm + 1, tr);
        t[v] = fn(t[v << 1], t[(v << 1) + 1], tl, tr);
    }
}

void update(int v, int tl, int tr, int pos, int val) {
    if (tl == tr) {
        t[v] = val == 0 ? br{1, 1, 1} : br{0, 0, 0};
    } else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm) {
            update(v << 1, tl, tm, pos, val);
        } else {
            update((v << 1) + 1, tm + 1, tr, pos, val);
        }
        t[v] = fn(t[v << 1], t[(v << 1) + 1], tl, tr);
    }
}

br get(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return {0, 0, 0};
    }
    if (l == tl && r == tr) {
        return t[v];
    }
    int tm = (tl + tr) >> 1;
    return fn(get(v << 1, tl, tm, l, min(r, tm)), get((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r), tl, tr);
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
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    build(a, 1, 0, n - 1);

    int m;
    cin >> m;
    while (m--) {
        string cmd;
        cin >> cmd;
        switch (cmd[0]) {
            case 'U': {
                int p, x;
                cin >> p >> x;
                update(1, 0, n - 1, p - 1, x);
                break;
            }
            case 'Q': {
                int l, r;
                cin >> l >> r;

                cout << get(1, 0, n - 1, l - 1, r - 1).ln << '\n';
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}