#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

graph g;
vector<int> used;

bool dfs(int v, int clr) {
    used[v] = clr;

    for (auto u : g[v]) {
        if (used[u] == clr) {
            return false;
        }

        if (used[u] != -1) {
            continue;
        }

        if (!dfs(u, (clr + 1) % 2)) {
            return false;
        }
    }

    return true;
}

int main() {
    int n, m;
    cin >> n >> m;

    g.resize(n);
    used.resize(n, -1);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].push_back(y);
        g[y].push_back(x);
    }

    for (int i = 0; i < n; ++i) {
        if (used[i] == -1) {
            if (!dfs(i, 0)) {
                cout << -1 << '\n';
                return 0;
            }
        }
    }

    for (auto i : used) {
        cout << i + 1 << ' ';
    }

    return 0;
}
