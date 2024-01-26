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
#include <unordered_map>
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

vector<vector<int>> g;
set<int> nodes;

int main() {
    int n;
    cin >> n;

    deque<int> code;
    unordered_map<int, int> cnt;
    for (int i = 0; i < n - 2; ++i) {
        int x;
        cin >> x;
        --x;
        code.push_back(x);
        ++cnt[x];
    }

    for (int i = 0; i < n; ++i) {
        if (cnt[i] == 0) {
            nodes.insert(i);
        }
    }

    g.resize(n);

    while (!code.empty()) {
        int u = code[0];
        int v = *nodes.begin();

        g[v].push_back(u);

        nodes.erase(nodes.begin());
        if (--cnt[code.front()] == 0) {
            nodes.insert(code.front());
        }
        code.pop_front();
    }

    int u = *nodes.begin();
    int v = *nodes.rbegin();

    g[v].push_back(u);

    for (int i = 0; i < n; ++i) {
        for (auto j : g[i]) {
            cout << i + 1 << ' ' << j + 1 << '\n';
        }
    }

    return 0;
}
