#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

graph g;

bool topsort(vector<int>& result) {
    result.clear();

    vector<int> prev(g.size());
    vector<int> used(g.size());
    int start = -1;
    int end = -1;

    function<bool(int)> dfs = [&start, &end, &prev, &used, &dfs](int v) {
        used[v] = 1;
        for (auto u : g[v]) {
            if (used[u] == 0) {
                prev[u] = v;
                if (dfs(u)) {
                    return true;
                }
            } else if (used[u] == 1) {
                end = v;
                start = u;
                return true;
            }
        }
        used[v] = 2;
        return false;
    };

    for (int i = 0; i < g.size(); ++i) {
        if (dfs(i)) {
            break;
        }
    }

    if (start == -1) {
        return false;
    } else {
        result.push_back(start);
        for (int v = end; v != start; v = prev[v]) {
            result.push_back(v);
        }
        reverse(result.begin(), result.end());

        return true;
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    g.resize(n);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].push_back(y);
    }

    vector<int> cycle;
    if (topsort(cycle)) {
        cout << cycle.size() << '\n';
        for (auto v : cycle) {
            cout << v + 1 << ' ';
        }
        cout << '\n';
    } else {
        cout << -1 << '\n';
    }

    return 0;
}
