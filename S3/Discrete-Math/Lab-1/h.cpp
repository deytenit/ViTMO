#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e3 + 100;

bool is_null(const vector<set<int>>& g) {
    for (const auto& i : g) {
        if (!i.empty()) {
            return false;
        }
    }

    return true;
}

vector<set<int>> sub(vector<set<int>> g) {
    for (int i = 0; i < g.size(); ++i) {
        if (!g[i].empty()) {
            int u = i;
            int v = *g[i].begin();

            g[v].erase(u);
            g[i].erase(v);
            break;
        }
    }

    return g;
}

vector<set<int>> compose(vector<set<int>> g) {
    int u, v;

    for (int i = 0; i < g.size(); ++i) {
        if (!g[i].empty()) {
            u = i;
            v = *g[i].begin();
            break;
        }
    }

    for (auto k : g[v]) {
        g[k].erase(v);

        if (k != u) {
            g[k].insert(u);
            g[u].insert(k);
        }
    }

    g.erase(next(g.begin(), v));

    for (auto& i : g) {
        set<int> tmp;

        for (auto j : i) {
            tmp.insert(j > v ? j - 1 : j);
        }

        i = move(tmp);
    }

    return g;
}

vector<int> null_decomposition(const vector<set<int>>& g, int k) {
    if (is_null(g)) {
        vector<int> coefficents(k);
        ++coefficents[g.size()];
        return coefficents;
    }

    auto subbed = null_decomposition(sub(g), k);
    auto composed = null_decomposition(compose(g), k);

    for (int i = 0; i < k; ++i) {
        subbed[i] -= composed[i];
    }

    return subbed;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    vector<set<int>> g(n);

    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        --x, --y;
        g[x].insert(y);
        g[y].insert(x);
    }

    auto res = null_decomposition(g, g.size() + 1);
    reverse(res.begin(), res.end());

    cout << g.size() << '\n';
    for (auto i : res) {
        cout << i << ' ';
    }

    return 0;
}
