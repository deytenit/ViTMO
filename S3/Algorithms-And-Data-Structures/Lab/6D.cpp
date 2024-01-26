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

void ford(int s, vector<int>& dist) {
    dist[s] = 0;
    while (true) {
        bool any = false;
        for (edge vu : g) {
            if (dist[vu.from] < INF)
                if (dist[vu.to] > dist[vu.from] + vu.weight) {
                    dist[vu.to] = dist[vu.from] + vu.weight;
                    any = true;
                }
        }
        if (!any) {
            break;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        int x, y, w;
        cin >> x >> y >> w;
        --x, --y;

        g.emplace_back(x, y, w);
        g.emplace_back(y, x, w);
    }

    vector<int> dist(n, INF);
    ford(0, dist);

    for (auto& i : dist) {
        cout << i << ' ';
    }
    cout << '\n';

    return 0;
}
