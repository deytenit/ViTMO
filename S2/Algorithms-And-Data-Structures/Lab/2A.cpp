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
int t[MAXN << 2];
int k[MAXN << 2];

void build(int a[], int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl];
    } else {
        int tm = (tl + tr) >> 1;
        build(a, v << 1, tl, tm);
        build(a, (v << 1) + 1, tm + 1, tr);
        t[v] = max(t[v << 1], t[(v << 1) + 1]);
    }
}

void push(int v) {
    t[v << 1] += k[v];
    k[v << 1] += k[v];
    t[(v << 1) + 1] += k[v];
    k[(v << 1) + 1] += k[v];
    k[v] = 0;
}

void update(int v, int tl, int tr, int l, int r, int add) {
    if (l > r) {
        return;
    }
    if (l == tl && tr == r) {
        t[v] += add;
        k[v] += add;
    } else {
        push(v);
        int tm = (tl + tr) >> 1;
        update(v << 1, tl, tm, l, min(r, tm), add);
        update((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r, add);
        t[v] = max(t[v << 1], t[(v << 1) + 1]);
    }
}

int get_max(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return -INF;
    }
    if (l == tl && tr == r) {
        return t[v];
    }
    push(v);
    int tm = (tl + tr) >> 1;
    return max(get_max(v << 1, tl, tm, l, min(r, tm)),
               get_max((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r));
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
        char cmd;
        cin >> cmd;
        switch (cmd) {
            case 'a': {
                int l, r, x;
                cin >> l >> r >> x;
                update(1, 0, n - 1, l - 1, r - 1, x);
                break;
            }
            case 'm': {
                int l, r;
                cin >> l >> r;
                cout << get_max(1, 0, n - 1, l - 1, r - 1) << '\n';
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}