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

constexpr int INF = 0x7fffffff;
constexpr ll MOD = 1e9 + 7;
constexpr ld EPS = 1e-8;
constexpr ld PI = 3.14159265358979323;
constexpr int MAXN = 2e5 + 1000;
constexpr int K = 17;

int a[MAXN];
int st[K][MAXN];
int p[MAXN + 1];

int fn(int x, int y) {
    return a[x] < a[y] ? x : y;
}

void build(int n) {
    p[1] = 0;
    for (int i = 2; i <= MAXN; i++) {
        p[i] = p[i >> 1] + 1;
    }

    iota(st[0], st[0] + n, 0);

    for (int i = 1; i <= K; ++i) {
        for (int j = 0; j + (1 << i) <= n; ++j) {
            st[i][j] = fn(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
        }
    }
}

int main() {
#ifdef _LOCAL
    const auto start = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();
    int n, m;
    cin >> n >> m >> a[0];

    for (int i = 1; i < n; ++i) {
        a[i] = (23 * a[i - 1] + 21563) % 16714589;
    }

    build(n);

    int u, v, ans;
    cin >> u >> v;

    for (int i = 0; i < m; ++i) {
        int l = u;
        int r = v;

        if (l > r) {
            swap(l, r);
        }

        --l, --r;
        int k = p[r - l + 1];
        int mx_i = fn(st[k][l], st[k][r - (1 << k) + 1]);
        ans = a[mx_i];
        if (i != m - 1) {
            u = ((17 * u + 751 + ans + 2 * (i + 1)) % n) + 1;
            v = ((13 * v + 593 + ans + 5 * (i + 1)) % n) + 1;
        }
    }

    cout << u << ' ' << v << ' ' << ans << '\n';
#ifdef _LOCAL
    cerr << "Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - start).count() << " ms.\n";
#endif
    return 0;
}