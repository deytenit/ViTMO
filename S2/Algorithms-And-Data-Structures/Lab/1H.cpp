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
constexpr int MAXN = 1e6 + 1000;

struct br {
    int ln = 0;
    int lb = 0;
    int rb = 0;
};

br t[MAXN << 2];

br fn(const br& x, const br& y) {
    int tmp = min(x.lb, y.rb);
    return {x.ln + y.ln + tmp, x.lb + y.lb - tmp, x.rb + y.rb - tmp};
}

void build(const string& str, int v, int tl, int tr) {
    if (tl == tr) {
        if (str[tl] == '(') {
            t[v].lb = 1;
        } else {
            t[v].rb = 1;
        }
    } else {
        int tm = (tl + tr) >> 1;
        build(str, v << 1, tl, tm);
        build(str, (v << 1) + 1, tm + 1, tr);
        t[v] = fn(t[v << 1], t[(v << 1) + 1]);
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
    return fn(get(v << 1, tl, tm, l, min(r, tm)), get((v << 1) + 1, tm + 1, tr, max(l, tm + 1), r));
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    string s;
    cin >> s;

    build(s, 1, 0, s.size() - 1);

    int m;
    cin >> m;
    while (m--) {
        int l, r;
        cin >> l >> r;
        cout << (get(1, 0, s.size() - 1, l - 1, r - 1).ln << 1) << '\n';
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}