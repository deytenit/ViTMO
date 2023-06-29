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
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace chrono;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

#define ff first
#define ss second
#define sqr(a) ((a) * (a))
#define vout(a) cerr << #a << ": " << (a) << '\n'

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;
const int K = 17;

int a[MAXN];
int st[K][MAXN];
int p[MAXN + 1];

int fn(int x, int y) {
    return a[x] > a[y] ? x : y;
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
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    build(n);

    int q;
    cin >> q;
    while (q--) {
        int l, r;
        cin >> l >> r;
        --l, --r;
        int i = p[r - l + 1];
        int mx_i = fn(st[i][l], st[i][r - (1 << i) + 1]);
        cout << a[mx_i] << ' ' << mx_i + 1 << '\n';
    }
#ifdef _LOCAL
    cerr << "Runtime: " << std::chrono::duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - start).count() << " ms.\n";
#endif
    return 0;
}