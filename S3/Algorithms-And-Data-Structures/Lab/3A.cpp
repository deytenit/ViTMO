#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

using graph = vector<vector<pair<int, int>>>;

graph g;

struct multiedge {
    int& get(const pair<int, int>& at) {
        return (_data.count(at) == 1 ? _data[at] : _data[{at.second, at.first}]);
    }

    map<pair<int, int>, int> _data;
};

multiedge me;

void find_bridges(vector<int>& result) {
    int timer = 0;
    vector<bool> used(g.size(), false);
    vector<int> tin(g.size(), -1);
    vector<int> low(g.size(), -1);

    function<void(int, int)> dfs = [&dfs, &used, &tin, &low, &timer, &result](int v, int p) {
        used[v] = true;
        tin[v] = low[v] = timer++;
        for (auto vu : g[v]) {
            int u = vu.first;
            if (u == p) {
                continue;
            }

            if (used[u]) {
                low[v] = min(low[v], tin[u]);
            } else {
                dfs(u, v);
                low[v] = min(low[v], low[u]);
                if (low[u] > tin[v] && me.get({u, v}) == 1) {
                    result.push_back(vu.second);
                }
            }
        }
    };

    for (int i = 0; i < g.size(); ++i) {
        if (!used[i]) {
            dfs(i, -1);
        }
    }

    sort(result.begin(), result.end());
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

        ++me.get({x, y});

        g[x].emplace_back(y, i);
        g[y].emplace_back(x, i);
    }

    vector<int> results;
    find_bridges(results);

    cout << results.size() << '\n';
    for (auto i : results) {
        cout << i + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
