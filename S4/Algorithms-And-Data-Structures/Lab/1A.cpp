#include <iostream>
#include <vector>

using namespace std;

int n, k;
vector<vector<int>> g;
vector<int> mt;
vector<char> used;

bool kuhn(int v) {
  if (used[v]) {
    return false;
  }
  used[v] = true;
  for (size_t i = 0; i < g[v].size(); ++i) {
    int to = g[v][i];
    if (mt[to] == -1 || kuhn(mt[to])) {
      mt[to] = v;
      return true;
    }
  }
  return false;
}

int main() {
  cin >> n >> k;
  g.resize(n);

  for (int i = 0; i < n; ++i) {
    int x;
    while (true) {
      cin >> x;
      if (x == 0) {
        break;
      }

      g[i].push_back(--x);
    }
  }

  mt.assign(k, -1);
  vector<char> used1(n);
  for (int i = 0; i < n; ++i) {
    for (size_t j = 0; j < g[i].size(); ++j) {
      if (mt[g[i][j]] == -1) {
        mt[g[i][j]] = i;
        used1[i] = true;
        break;
      }
    }
  }

  for (int i = 0; i < n; ++i) {
    if (used1[i]) {
      continue;
    }
    used.assign(n, false);
    kuhn(i);
  }

  vector<pair<int, int>> res;

  for (int i = 0; i < k; ++i) {
    if (mt[i] != -1) {
      res.emplace_back(mt[i] + 1, i + 1);
    }
  }

  cout << res.size() << '\n';
  for (auto &it : res) {
    cout << it.first << ' ' << it.second << '\n';
  }
}
