#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e3 + 100;

vector<vector<int>> g;
vector<bool> used;

void dfs(int v, vector<int>& ans) {
    used[v] = true;
    for (auto u : g[v]) {
        if (!used[u]) {
            dfs(u, ans);
        }
    }

    ans.push_back(v);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    g.resize(n);
    used.resize(n);

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            char x;
            cin >> x;

            if (x == '1') {
                g[i].push_back(j);
            } else {
                g[j].push_back(i);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        vector<int> ans;

        fill(used.begin(), used.end(), false);

        dfs(i, ans);

        reverse(ans.begin(), ans.end());

        if (find(g[ans.back()].begin(), g[ans.back()].end(), ans.front()) != g[ans.back()].end()) {
            for (auto v : ans) {
                cout << v + 1 << ' ';
            }
            cout << '\n';
            break;
        }
    }

    return 0;
}
