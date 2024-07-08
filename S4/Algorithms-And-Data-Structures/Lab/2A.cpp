#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

const int MAXN = 510;

int n, k;
vector<int> g[MAXN];
int match[MAXN], p[MAXN], base[MAXN], q[MAXN];
bool used[MAXN], blossom[MAXN];

int lca(int a, int b) {
  bool used[MAXN] = {0};
  for (;;) {
    a = base[a];
    used[a] = true;
    if (match[a] == -1) {
      break;
    }
    a = p[match[a]];
  }
  for (;;) {
    b = base[b];
    if (used[b]) {
      return b;
    }
    b = p[match[b]];
  }
}

void mark_path(int v, int b, int children) {
  while (base[v] != b) {
    blossom[base[v]] = blossom[base[match[v]]] = true;
    p[v] = children;
    children = match[v];
    v = p[match[v]];
  }
}

int find_path(int root) {
  fill(used, used + n, false);
  fill(p, p + n, -1);
  iota(base, base + n, 0);

  used[root] = true;
  int qh = 0, qt = 0;
  q[qt++] = root;

  while (qh < qt) {
    int v = q[qh++];
    for (size_t i = 0; i < g[v].size(); ++i) {
      int to = g[v][i];
      if (base[v] == base[to] || match[v] == to) {
        continue;
      }

      if (to == root || match[to] != -1 && p[match[to]] != -1) {
        int curbase = lca(v, to);
        fill(blossom, blossom + n, 0);
        mark_path(v, curbase, to);
        mark_path(to, curbase, v);
        for (int i = 0; i < n; ++i)
          if (blossom[base[i]]) {
            base[i] = curbase;
            if (!used[i]) {
              used[i] = true;
              q[qt++] = i;
            }
          }
      } else if (p[to] == -1) {
        p[to] = v;
        if (match[to] == -1) {
          return to;
        }
        to = match[to];
        used[to] = true;
        q[qt++] = to;
      }
    }
  }
  return -1;
}

int main() {
  cin >> n >> k;

  for (int i = 0; i < k; ++i) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    g[x].push_back(y);
    g[y].push_back(x);
  }

  fill(match, match + n, -1);
  for (int i = 0; i < n; ++i) {
    if (match[i] == -1) {
      int v = find_path(i);
      while (v != -1) {
        int pv = p[v], ppv = match[pv];
        match[v] = pv, match[pv] = v;
        v = ppv;
      }
    }
  }

  vector<bool> used(n, false);
  vector<pair<int, int>> res;

  for (int i = 0; i < n; ++i) {
    if (match[i] == -1 || used[i]) {
      continue;
    }
    used[i] = used[match[i]] = true;

    res.emplace_back(i + 1, match[i] + 1);
  }

  cout << res.size() << '\n';
  for (auto &it : res) {
    cout << it.first << ' ' << it.second << '\n';
  }
}
