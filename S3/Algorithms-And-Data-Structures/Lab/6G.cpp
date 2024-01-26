#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

struct edge {
    edge(int from, int to, int weight) : from(from), to(to), weight(weight) {}

    int from, to, weight;
};

using graph = vector<edge>;

const int INF = 0x7fffffff;

graph g;

bool ford(int s, int k, vector<int>& dist) {
    bool any;
    dist[s] = 0;
    for (int i = 0; i < k; ++i) {
        any = false;
        vector<int> tmp(dist.size(), INF);
        for (edge vu : g) {
            if (dist[vu.from] < INF) {
                if (tmp[vu.to] > dist[vu.from] + vu.weight) {
                    tmp[vu.to] = dist[vu.from] + vu.weight;
                    any = true;
                }
            }
        }

        dist = tmp;
    }

    return any;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, s;
    cin >> n >> m >> k >> s;
    --s;

    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        --x, --y;

        g.emplace_back(x, y, w);
    }

    vector<int> dist(n, INF);
    ford(s, k, dist);

    for (int i = 0; i < dist.size(); ++i) {
        cout << (i == s || dist[i] == INF ? -1 : dist[i]) << '\n';
    }

    return 0;
}
