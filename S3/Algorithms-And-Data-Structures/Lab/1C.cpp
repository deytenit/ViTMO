#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

graph g;
vector<bool> used;

void dfs(int v, vector<int>& comp) {
    used[v] = true;
    comp.push_back(v);

    for (auto u : g[v]) {
        if (used[u]) {
            continue;
        }

        dfs(u, comp);
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    g.resize(n);
    used.resize(n, false);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].push_back(y);
        g[y].push_back(x);
    }

    vector<vector<int>> comps;

    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            comps.emplace_back();
            dfs(i, comps.back());
        }
    }

    cout << comps.size() - 1 << '\n';
    for (int i = 0; i < comps.size() - 1; ++i) {
        cout << comps[i].front() + 1 << ' ' << comps[i + 1].front() + 1 << '\n';
    }

    return 0;
}
