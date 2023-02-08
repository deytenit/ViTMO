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

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    vector<int> a;

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        a.push_back(x);
    }

    vector<int> dp(n, 1);
    vector<int> pred(n, 0);

    for (int i = 1; i < a.size(); ++i) {
        pred[i] = i;
        for (int j = 0; j < i; ++j) {
            if (a[i] > a[j]) {
                if (dp[j] + 1 > dp[i]) {
                    pred[i] = j;
                    dp[i] = dp[j] + 1;
                }
            }
        }
    }

    int mx_p = 0;
    int mx = dp[0];

    for (int i = 1; i < dp.size(); ++i) {
        if (mx < dp[i]) {
            mx = dp[i];
            mx_p = i;
        }
    }

    cout << mx << '\n';

    vector<int> result;

    while (pred[mx_p] != mx_p) {
        result.push_back(a[mx_p]);
        mx_p = pred[mx_p];
    }

    result.push_back(a[mx_p]);

    reverse(result.begin(), result.end());

    for (int it : result) {
        cout << it << ' ';
    }

    cout << '\n';

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
