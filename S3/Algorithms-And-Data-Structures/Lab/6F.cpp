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

const int INF = 1e7;

graph g;

bool ford() {
    vector<int> dist(g.size(), INF);
    bool any;

    for (int i = 0; i < dist.size(); ++i) {
        any = false;
        for (edge vu : g) {
            if (dist[vu.to] > dist[vu.from] + vu.weight) {
                dist[vu.to] = max(-INF, dist[vu.from] + vu.weight);
                any = true;
            }
        }
    }

    return any;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int w;
            cin >> w;

            if (w != 1e5) {
                g.emplace_back(i, j, w);
            }
        }
    }

    cout << (ford() ? "YES" : "NO") << '\n';

    return 0;
}
