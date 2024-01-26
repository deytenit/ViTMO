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

int ford(vector<int>& prev) {
    int end;
    vector<int> dist(g.size(), INF);

    for (int i = 0; i < dist.size(); ++i) {
        end = -1;
        for (edge vu : g) {
            if (dist[vu.to] > dist[vu.from] + vu.weight) {
                dist[vu.to] = max(-INF, dist[vu.from] + vu.weight);
                prev[vu.to] = vu.from;
                end = vu.to;
            }
        }
    }

    return end;
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

            if (i != j && w != 1e5) {
                g.emplace_back(i, j, w);
            }
        }
    }

    vector<int> prev(n, -1);
    int end = ford(prev);

    if (end == -1) {
        cout << "No" << '\n';
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        end = prev[end];
    }

    vector<int> path;
    for (int v = end; v != end || path.empty(); v = prev[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    cout << "YES" << '\n';
    cout << path.size() << '\n';
    for (auto i : path) {
        cout << i + 1 << ' ';
    }
    cout << '\n';

    return 0;
}
