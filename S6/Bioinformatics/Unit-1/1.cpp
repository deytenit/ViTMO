#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> g;

vector<int> eulerian_cycle(const int s) {
  vector<int> res;

  vector<int> st;
  st.push_back(s);

  while (!st.empty()) {
    int v = st.back();

    if (g[v].size() == 0) {
      res.push_back(v);
      st.pop_back();
    } else {
      int u = g[v].back();
      g[v].pop_back();

      st.push_back(u);
    }
  }

  reverse(res.begin(), res.end());
  res.pop_back();
  return res;
}

int main() {
  int n, m;
  cin >> n >> m;

  g.resize(n);

  for (int i = 0; i < m; ++i) {
    int x, y;
    cin >> x >> y;
    --x, --y;

    g[x].push_back(y);
  }

  vector<int> res = eulerian_cycle(0);

  for (const int it: res) {
    cout << it + 1 << ' ';
  }

  return 0;
}
