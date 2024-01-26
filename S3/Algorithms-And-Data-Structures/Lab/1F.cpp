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

    vector<int> used(g.size());
    bool cycle = false;

    function<bool(int)> dfs = [&cycle, &result, &used, &dfs](int v) {
        used[v] = 1;
        for (auto u : g[v]) {
            if (used[u] == 0) {
                if (dfs(u)) {
                    return true;
                }
            } else if (used[u] == 1) {
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
    int n, m;
    cin >> n >> m;

    g.resize(n);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g[x].push_back(y);
    }

    vector<int> result;
    if (topsort(result)) {
        for (auto v : result) {
            cout << v + 1 << ' ';
        }
        cout << '\n';
    } else {
        cout << -1 << '\n';
    }

    return 0;
}
