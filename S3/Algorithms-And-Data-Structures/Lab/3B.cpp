#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

graph g;

void find_articulations(set<int>& result) {
    int timer = 0;
    vector<bool> used(g.size(), false);
    vector<int> tin(g.size(), -1);
    vector<int> low(g.size(), -1);

    function<void(int, int)> dfs = [&dfs, &used, &tin, &low, &timer, &result](int v, int p) {
        used[v] = true;
        tin[v] = low[v] = timer++;
        int children = 0;
        for (size_t i = 0; i < g[v].size(); ++i) {
            int to = g[v][i];
            if (to == p)
                continue;
            if (used[to])
                low[v] = min(low[v], tin[to]);
            else {
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] >= tin[v] && p != -1) {
                    result.insert(v);
                }
                ++children;
            }
        }
        if (p == -1 && children > 1) {
            result.insert(v);
        }
    };

    for (int i = 0; i < g.size(); ++i) {
        if (!used[i]) {
            dfs(i, -1);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    g.resize(n);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].push_back(y);
        g[y].push_back(x);
    }

    set<int> results;
    find_articulations(results);

    cout << results.size() << '\n';
    for (auto i : results) {
        cout << i + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
