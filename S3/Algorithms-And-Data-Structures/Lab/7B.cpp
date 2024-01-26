#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

#define int long long

struct edge {
    edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}

    bool operator<(edge const& other) {
        return weight < other.weight;
    }

    int from, to, weight;
};

using graph = vector<edge>;

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

    sort(g.begin(), g.end());

    for (auto& vu : g) {
        int v = vu.from;
        int u = vu.to;
        if (p.lead(v) != p.lead(u)) {
            res.push_back(vu);
            p.unite(v, u);
        }
    }

    return res;
}

signed main() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        --x, --y;

        g.emplace_back(x, y, w);
        g.emplace_back(y, x, w);
    }

    graph tree{kruskal(n)};

    cout << accumulate(tree.begin(), tree.end(), 0LL, [](int sum, const edge& next) { return sum + next.weight; }) << '\n';

    return 0;
}
