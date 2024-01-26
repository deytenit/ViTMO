// ЛШКН - 2020

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0);                       \
    cout.tie(0);
#define fstr()                 \
    ifstream cin("input.txt"); \
    ofstream cout("output.txt");

#define pb push_back
#define sqr(a) ((a) * (a))
#define fp first
#define sp second
#define mp make_pair

typedef long long ll;
typedef long double ld;

const int INF = 2e9;
const int MOD = 1e9 + 7;
const ld EPS = 1e-10;
const ld PI = 3.14159265358979323;

int t = 0;
int n, m;
vector<vector<int> > g;
int color[205000];
int up[205000];
int d[205000];
int ch[205000];
bool r[205000];

void dfs(int v) {
    color[v] = 1;
    t++;
    d[v] = t;
    up[v] = n + m + 1;
    for (int i = 0; i < g[v].size(); i++) {
        if (color[g[v][i]] == 0) {
            ch[v]++;
            dfs(g[v][i]);
            up[v] = min(up[v], up[g[v][i]]);
            if (up[g[v][i]] >= d[v])
                r[v] = true;
        } else
            up[v] = min(up[v], d[g[v][i]]);
    }
}

int main() {
    fast();
    cin >> n >> m;
    g.resize(m + n);
    for (int i = 0; i < m + n; i++) {
        color[i] = 0;
        d[i] = 0;
        up[i] = 0;
        ch[i] = 0;
        r[i] = false;
    }
    for (int i = 0; i < m; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        x += m;
        y += m;
        z += m;
        g[i].pb(x - 1);
        g[x - 1].pb(i);
        g[i].pb(y - 1);
        g[y - 1].pb(i);
        g[i].pb(z - 1);
        g[z - 1].pb(i);
    }
    for (int i = 0; i < n + m; i++) {
        if (color[i] == 0) {
            dfs(i);
            if (ch[i] > 1)
                r[i] = true;
            else
                r[i] = false;
        }
    }
    int num = 0;
    for (int i = 0; i < m; i++) {
        if (r[i]) {
            num++;
        }
    }
    cout << num << "\n";
    for (int i = 0; i < m; i++) {
        if (r[i]) {
            cout << i + 1 << " ";
        }
    }
    cout << "\n";
    return 0;
}
