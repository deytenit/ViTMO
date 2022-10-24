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

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e5 + 1000;

vector<int> a;

vector<int> check(long long sum, int k) {
    vector<int> ans;
    vector<int> add;
    long long temp = 0;

    for (int i = 0; i < a.size(); i++) {
        if (temp + a[i] <= sum) {
            if (i != 0) {
                add.push_back(i);
            }
            temp += a[i];
        } else {
            ans.push_back(i);
            temp = a[i];
        }
    }

    int i = 0;
    while (ans.size() < k - 1) {
        ans.push_back(add[i++]);
    }

    sort(ans.begin(), ans.end());

    return ans;
}

long long lbinary(long long l, long long r, int x) {
    while (r - l > 1) {
        long long m = (l + r) / 2;

        if (x - 1 < check(m, x).size()) {
            l = m;
        } else {
            r = m;
        }
    }

    return r;
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n, k;

    cin >> n >> k;

    long long s = 0;
    int mx = 0;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        a.push_back(x);
        s += x;
        mx = max(x, mx);
    }

    for (int it : check(lbinary(mx - 1, s, k), k)) {
        cout << it << ' ';
    }

    cout << '\n';

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}