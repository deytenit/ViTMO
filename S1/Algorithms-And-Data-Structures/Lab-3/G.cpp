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

const int INF = 2e9;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;

int n;
vector<long double> a;

bool check() {
    for (int i = 2; i < n; i++) {
        if ((a[i] = 2 * a[i - 1] - a[i - 2] + 2) <= EPS) {
            return false;
        }
    }

    return true;
}

long double binary(long double l, long double r) {
    for (int i = 0; i < 200; i++) {
        long double m = (l + r) / 2;
        a[1] = m;
        if (!check()) {
            l = m;
        } else {
            r = m;
        }
    }

    return a[n - 1];
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    cin >> n;
    a.resize(n);

    cin >> a[0];

    cout << fixed << setprecision(2) << binary(0, a[0]) << '\n';

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}