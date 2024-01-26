#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<vector<pair<int, int>>>;

const int INF = 0x7fffffff;

graph g;

bool topsort(vector<int>& result) {
    result.clear();

    vector<int> used(g.size());
    bool cycle = false;

    function<bool(int)> dfs = [&cycle, &result, &used, &dfs](int v) {
        used[v] = 1;
        for (auto u : g[v]) {
            if (used[u.first] == 0) {
                if (dfs(u.first)) {
                    return true;
                }
            } else if (used[u.first] == 1) {
                cycle = true;
                return true;
            }
        }
        used[v] = 2;
        result.push_back(v);
        return false;
    };

    for (int i = 0; i < g.size(); ++i) {
        if (used[i] != 0) {
            continue;
        }

        if (dfs(i)) {
            break;
        }
    }

    if (cycle) {
        return false;
    } else {
        reverse(result.begin(), result.end());

        return true;
    }
}

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    --s, --t;

    g.resize(n);

    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        --x, --y;

        g[x].push_back({y, w});
    }

    vector<int> result;
    topsort(result);

    vector<int> dist(n, INF);
    dist[s] = 0;
    for (auto v : result) {
        for (auto u : g[v]) {
            if (dist[v] == INF) {
                continue;
            }

            if (dist[u.first] > dist[v] + u.second) {
                dist[u.first] = dist[v] + u.second;
            }
        }
    }

    if (dist[t] == INF) {
        cout << "Unreachable" << '\n';
    } else {
        cout << dist[t] << '\n';
    }

    return 0;
}
