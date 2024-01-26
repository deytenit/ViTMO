#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e3 + 100;

vector<vector<int>> g;
vector<bool> used;
vector<int> colors;

void dfs(int v, int k) {
    used[v] = true;
    for (auto u : g[v]) {
        if (used[u]) {
            continue;
        }

        vector<bool> reserved(k);

        for (auto s : g[u]) {
            if (colors[s] != -1) {
                reserved[colors[s]] = true;
            }
        }

        colors[u] = distance(reserved.begin(), find(reserved.begin(), reserved.end(), false));

        dfs(u, k);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    g.resize(n);
    used.resize(n);
    colors.resize(n, -1);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    int k = accumulate(g.begin(), g.end(), -INF, [](int curr, const vector<int>& next) { return max(curr, static_cast<int>(next.size())); });
    k += (k + 1) % 2;

    colors[0] = 0;
    dfs(0, k);

    cout << k << '\n';
    for (auto color : colors) {
        cout << color + 1 << '\n';
    }

    return 0;
}
