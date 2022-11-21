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

vector<string> a;

void radix(vector<string>& a, int i) {
    vector<string> tmp(a.size());
    vector<int> chr(256);

    for (int j = 0; j < a.size(); ++j) {
        ++chr[a[j][i]];
    }

    int cnt = 0;

    for (int j = 0; j < chr.size(); ++j) {
        int t = chr[j];
        chr[j] = cnt;
        cnt += t;
    }

    for (int j = 0; j < a.size(); ++j) {
        tmp[chr[a[j][i]]] = a[j];
        ++chr[a[j][i]];
    }

    a = tmp;
}

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n, m, k;
    cin >> n >> m >> k;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        reverse(s.begin(), s.end());
        a.push_back(s);
    }

    for (int i = 0; i < k; ++i) {
        radix(a, i);
    }

    for (auto it : a) {
        reverse(it.begin(), it.end());
        cout << it << '\n';
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}
