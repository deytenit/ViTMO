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

int a[MAXN];
int t[MAXN << 2];
int k[MAXN << 2];

void build(int a[], int v, int tl, int tr) {
    if (tl == tr) {
        t[v] = a[tl] == 0;
    } else {
        int tm = (tl + tr) >> 1;
        build(a, v << 1, tl, tm);
        build(a, (v << 1) + 1, tm + 1, tr);
        t[v] = t[v << 1] + t[(v << 1) + 1];
    }
}

void update(int v, int tl, int tr, int pos, int val) {
    if (tl == tr) {
        t[v] = val == 0;
    } else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm) {
            update(v << 1, tl, tm, pos, val);
        } else {
            update((v << 1) + 1, tm + 1, tr, pos, val);
        }
        t[v] = t[v << 1] + t[(v << 1) + 1];
    }
}

int get_max(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return -INF;
    }
    if (l == tl && tr == r) {
        return t[v];
    }
    int tm = (tl + tr) >> 1;
    return max(get_max(v << 1, tl, tm, l, min(r, tm)),
               get_max((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r));
}

int sum(int v, int tl, int tr, int l, int r) {
    if (l > r) {
        return 0;
    }
    if (l == tl && r == tr) {
        return t[v];
    }
    int tm = (tl + tr) >> 1;
    return sum(v << 1, tl, tm, l, min(r, tm)) + sum((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r);
}

int find_kth(int v, int tl, int tr, int k) {
    if (k > t[v]) {
        return -1;
    }
    if (tl == tr) {
        return tl;
    }
    int tm = (tl + tr) >> 1;
    if (t[v << 1] >= k) {
        return find_kth(v << 1, tl, tm, k);
    } else {
        return find_kth((v << 1) + 1, tm + 1, tr, k - t[v << 1]);
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
            case 'u': {
                int p, x;
                cin >> p >> x;
                update(1, 0, n - 1, p - 1, x);
                break;
            }
            case 's': {
                int l, r, x;
                cin >> l >> r >> x;

                if (sum(1, 0, n - 1, l - 1, r - 1) < x) {
                    cout << -1 << '\n';
                    break;
                }

                int q = l > 1 ? sum(1, 0, n - 1, 0, l - 2) : 0;
                cout << find_kth(1, 0, n - 1, q + x) + 1 << '\n';
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}