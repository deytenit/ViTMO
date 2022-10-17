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

vector<pair<int, bool>> a;

void merge(vector<pair<int, bool>>& a, int begin, int mid, int end) {
    int l = begin;
    int r = mid;

    vector<pair<int, bool>> merged;

    while (l < mid || r < end) {
        if (r == end || (l < mid && a[l] <= a[r])) {
            merged.push_back(a[l]);
            l++;
        } else if (l == mid || (r < end && a[r] < a[l])) {
            merged.push_back(a[r]);
            r++;
        }
    }

    for (int i = begin; i < end; i++) {
        a[i] = merged[i - begin];
    }
}

void merge_sort(vector<pair<int, bool>>& a, int l, int r) {
    if (r - l == 1) {
        return;
    }

    int m = (l + r) / 2;

    merge_sort(a, l, m);
    merge_sort(a, m, r);

    merge(a, l, m, r);
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
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        a.push_back({x, false});
        a.push_back({y, true});
    }

    merge_sort(a, 0, a.size());

    int ovrlp = 0;
    int bg = -1;
    int ans = 0;

    for (int i = 0; i < a.size(); i++) {
        if (!a[i].second) {
            if (ovrlp == 0) {
                bg = i;
            }
            ovrlp++;
        } else {
            if (ovrlp == 1) {
                ans += a[i].first - a[bg].first + 1;
            }
            ovrlp--;
        }
    }

    cout << ans << '\n';

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}