#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<vector<int>>;

const int INF = 30000;

graph g;

void floyd(int n) {
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (g[i][k] < INF && g[k][j] < INF) {
                    g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    g.resize(n, vector<int>(n, INF));

    for (int i = 0; i < n; ++i) {
        g[i][i] = 0;
    }

    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        --x, --y;

        if (x != y && g[x][y] > w) {
            g[x][y] = w;
        }
    }

    floyd(n);

    for (auto& rw : g) {
        for (auto it : rw) {
            cout << it << ' ';
        }
        cout << '\n';
    }

    return 0;
}
