#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
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
const int MAXN = 4e3 + 100;

vector<unordered_set<int>> g;
set<int> nodes;

int main() {
    int n;
    cin >> n;

    g.resize(n);

    for (int i = 0; i < n - 1; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].insert(y);
        g[y].insert(x);
    }

    for (int i = 0; i < n; ++i) {
        if (g[i].size() == 1) {
            nodes.insert(i);
        }
    }

    vector<int> ans;

    for (int i = 0; i < n - 2; ++i) {
        int u = *nodes.begin();
        nodes.erase(nodes.begin());

        int v = *g[u].begin();
        g[v].erase(u);

        if (g[v].size() == 1) {
            nodes.insert(v);
        }

        ans.push_back(v);
    }

    for (auto i : ans) {
        cout << i + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
