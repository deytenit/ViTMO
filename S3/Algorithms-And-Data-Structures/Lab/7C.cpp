// Codeforces - archive

#include <algorithm>
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

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(nullptr)

#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define sqr(a) ((a) * (a))
#define vout(a) cout << #a << ": " << (a) << '\n'

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int INF = 0x7fffffff;
const ll MOD = (1 << 30) + 1;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e6 + 1000;

ll s[MAXN], a[MAXN];

set<int> b;
vector<ll> ans;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    s[0] = 0;
    for (int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        s[i] = s[i - 1] + x;
    }
    s[n + 1] = s[n];
    ll mx = 0;
    for (int j = 0; j < n; j++) {
        int x;
        cin >> x;
        b.insert(x);
        a[j] = x;
    }
    for (auto i = b.begin(); i != b.end(); i++) {
        if (i == b.begin())
            mx = s[*i - 1];
        auto k = b.end();
        k--;
        if (i == k) {
            mx = max(mx, s[n] - s[*i]);
        }
        if (i != b.begin()) {
            auto t = i;
            t--;
            mx = max(mx, s[*i - 1] - s[*t]);
        }
    }
    ans.pb(0);
    for (int j = n - 1; j >= 1; j--) {
        auto i = b.find(a[j]);
        auto k = b.end();
        k--;
        if (i == b.begin()) {
            if (i != k) {
                auto t = i;
                t++;
                mx = max(mx, s[*t - 1]);
            } else {
                mx = max(mx, s[n]);
            }
            b.erase(i);
        } else if (i == k) {
            auto t = i;
            t--;
            mx = max(mx, s[n] - s[*t]);
            b.erase(i);
        } else {
            auto t1 = i;
            auto t2 = i;
            t1--;
            t2++;
            mx = max(mx, s[*t2 - 1] - s[*t1]);
            b.erase(i);
        }
        ans.pb(mx);
    }
    for (int i = ans.size() - 1; i >= 0; i--) {
        cout << ans[i] << '\n';
    }
    return 0;
}
