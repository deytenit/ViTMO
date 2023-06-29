#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
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

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;
const int K = 17;

int a[MAXN];
pair<int, int> t[MAXN << 2];

pair<int, int> fn(pair<int, int> x, pair<int, int> y) {
    return x.first > y.first ? x : x.first == y.first ? make_pair(x.first, x.second + y.second)
                                                      : y;
}

void build(int v, int L, int R) {
    if (L == R) {
        t[v] = {a[L], 1};
    } else {
        int M = (L + R) >> 1;
        build(v << 1, L, M);
        build((v << 1) + 1, M + 1, R);
        t[v] = fn(t[v << 1], t[(v << 1) + 1]);
    }
}

pair<int, int> rmq(int v, int L, int R, int l, int r) {
    if (l > r) {
        return {-INF, 0};
    }
    if (l == L && r == R) {
        return t[v];
    }
    int M = (L + R) >> 1;
    pair<int, int> tl = rmq(v << 1, L, M, l, min(r, M));
    pair<int, int> tr = rmq((v << 1) + 1, M + 1, R, max(l, M + 1), r);
    return fn(tl, tr);
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

    build(1, 0, n - 1);

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        pair<int, int> mx_i = rmq(1, 0, n - 1, l - 1, r - 1);
        cout << mx_i.first << ' ' << mx_i.second << '\n';
    }
#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}