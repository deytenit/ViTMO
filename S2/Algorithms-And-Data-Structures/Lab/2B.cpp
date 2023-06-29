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
constexpr int MAXN = 2e5 + 1000;
constexpr int CNT = 24;

#define int long long

int t[24][MAXN << 2];
bool k[24][MAXN << 2];
int a[MAXN];

void build(int a[], int q, int v, int tl, int tr) {
    if (tl == tr) {
        t[q][v] = (a[tl] >> q) & 1;
    } else {
        int tm = (tl + tr) >> 1;
        build(a, q, v << 1, tl, tm);
        build(a, q, (v << 1) + 1, tm + 1, tr);
        t[q][v] = t[q][v << 1] + t[q][(v << 1) + 1];
    }
}

void push(int q, int v, int l, int r) {
    if (k[q][v]) {
        k[q][v << 1] ^= 1;
        k[q][(v << 1) + 1] ^= 1;
        k[q][v] = 0;
        t[q][v] = r - l + 1 - t[q][v];
    }
}

int sum(int q, int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 0;
    }

    if (tl == l && tr == r) {
        if (k[q][v]) {
            return tr - tl + 1 - t[q][v];
        }
        return t[q][v];
    } else {
        push(q, v, tl, tr);
        int tm = (tl + tr) >> 1;
        return sum(q, v << 1, tl, tm, l, min(r, tm)) +
               sum(q, (v << 1) + 1, tm + 1, tr, max(l, tm + 1), r);
    }
}

void update(int q, int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return;
    }

    if (l == tl && r == tr) {
        k[q][v] ^= 1;
        return;
    } else {
        push(q, v, tl, tr);
        int tm = (tl + tr) >> 1;
        update(q, v << 1, tl, tm, l, min(r, tm));
        update(q, (v << 1) + 1, tm + 1, tr, max(l, tm + 1), r);
        t[q][v] = (k[q][v << 1] ? min(r, tm) - tl + 1 - t[q][v << 1] : t[q][v << 1]) + (k[q][(v << 1) + 1] ? tr - max(l, tm + 1) + 1 - t[q][(v << 1) + 1] : t[q][(v << 1) + 1]);
    }
}

signed main() {
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

    for (int j = 0; j < CNT; ++j) {
        build(a, j, 1, 0, n - 1);
    }

    int m;
    cin >> m;
    while (m--) {
        int cmd;
        cin >> cmd;
        switch (cmd) {
            case 1: {  // sum
                int l, r;
                cin >> l >> r;
                int ans = 0;
                for (int i = 0; i < CNT; ++i) {
                    ans += sum(i, 1, 0, n - 1, l - 1, r - 1) * (1 << i);
                }
                cout << ans << '\n';
                break;
            }
            case 2: {  // mod
                int l, r, x;
                cin >> l >> r >> x;
                for (int i = 0; i < CNT; ++i) {
                    if (x & (1 << i)) {
                        update(i, 1, 0, n - 1, l - 1, r - 1);
                    }
                }
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}