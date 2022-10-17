#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
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

random_device rd;
mt19937 mte(rd());

vector<int> a;

int partion(vector<int>& a, int l, int r) {
    int i = l;
    int j = r - 1;

    uniform_int_distribution<> distrib(i, j);
    int m = a[distrib(mte)];

    do {
        while (a[i] < m) {
            i++;
        }

        while (a[j] > m) {
            j--;
        }

        if (i <= j) {
            swap(a[i++], a[j--]);
        }
    } while (i <= j);

    return j + 1;
}

void qsort(vector<int>& a, int l, int r) {
    if (r - l > 1) {
        int m = partion(a, l, r);
        qsort(a, l, m);
        qsort(a, m, r);
    }
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
        int x;
        cin >> x;
        a.push_back(x);
    }

    qsort(a, 0, a.size());

    for (int it : a) {
        cout << it << ' ';
    }
    cout << '\n';

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}