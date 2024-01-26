#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using graph = vector<pair<int, int>>;

struct dsu {
    dsu(int n) : data(n), weight(n, 1) {
        iota(data.begin(), data.end(), 0);
    }

    int lead(int v) {
        return (v == data[v]) ? v : data[v] = lead(data[v]);
    }

    void unite(int a, int b) {
        a = lead(a);
        b = lead(b);

        if (weight[a] > weight[b]) {
            swap(a, b);
        }

        weight[b] += weight[a];
        data[a] = b;
    }

    vector<int> data;
    vector<int> weight;
};

graph g;

graph kruskal(int n) {
    graph res;
    dsu p{n};

    for (auto& vu : g) {
        int v = vu.first;
        int u = vu.second;
        if (p.lead(v) != p.lead(u)) {
            res.push_back(vu);
            p.unite(v, u);
        }
    }

    return res;
}

int main() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;

        g.emplace_back(x, y);
    }

    for (auto& vu : kruskal(n)) {
        cout << vu.first + 1 << ' ' << vu.second + 1 << '\n';
    }

    return 0;
}
