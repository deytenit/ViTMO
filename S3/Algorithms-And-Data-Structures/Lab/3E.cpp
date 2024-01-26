#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

using graph = vector<vector<pair<int, int>>>;

graph g;

int find_colors(vector<int>& result) {
    int timer = 0;
    int max_color = 0;
    vector<int> st;
    vector<bool> used(g.size(), false);
    vector<int> tin(g.size(), -1);
    vector<int> low(g.size(), -1);

    function<void(int)> paint = [&](int v) {
        ++max_color;
        int last = -1;
        while (last != v && !st.empty()) {
            result[st.back()] = max_color;
            last = st.back();
            st.pop_back();
        }
    };

    function<void(int)> dfs = [&](int v) {
        used[v] = true;
        st.push_back(v);
        tin[v] = low[v] = timer++;
        for (auto vu : g[v]) {
            int u = vu.first;

            if (used[u]) {
                low[v] = min(low[v], tin[u]);
            } else {
                dfs(u);
                low[v] = min(low[v], low[u]);
                if (low[u] > tin[v]) {
                    paint(vu.second);
                }
            }
        }
    };

    for (int i = 0; i < g.size(); ++i) {
        if (!used[i]) {
            dfs(i);
            paint(i);
        }
    }

    return max_color;
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

        g[x].emplace_back(y, i);
        g[y].emplace_back(x, i);
    }

    vector<int> results(n);
    int max_color = find_colors(results);

    cout << max_color << '\n';
    for (auto i : results) {
        cout << i + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
