#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

const int INF = 0x7fffffff;

graph g;

void bfs(int s, vector<int>& prev) {
    queue<int> q;

    q.push(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto u : g[v]) {
            if (prev[u] == -1) {
                prev[u] = v;
                q.push(u);
            }
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
    }

    int s, t;
    cin >> s >> t;
    --s, --t;

    vector<int> prev(n, -1);
    bfs(s, prev);

    if (prev[t] == -1) {
        cout << -1 << '\n';
        return 0;
    }

    vector<int> path;
    for (int v = t; v != s; v = prev[v]) {
        path.push_back(v);
    }
    path.push_back(s);

    reverse(path.begin(), path.end());

    cout << path.size() - 1 << '\n';
    for (auto i : path) {
        cout << i + 1 << ' ';
    }

    return 0;
}
