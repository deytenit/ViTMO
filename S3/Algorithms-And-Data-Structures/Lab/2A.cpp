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

int num;
vector<vector<int> > g;
vector<vector<int> > gt;
int color[22000];
int list[22000];

void dfs1(int v) {
    color[v] = 1;
    for (int i = 0; i < g[v].size(); i++) {
        if (color[g[v][i]] == 0)
            dfs1(g[v][i]);
    }
    num--;
    list[num] = v;
}

void dfs2(int v) {
    color[v] = num;
    for (int i = 0; i < gt[v].size(); i++) {
        if (color[gt[v][i]] == 0)
            dfs2(gt[v][i]);
    }
}

int main() {
    fast();
    int n, m;
    cin >> n >> m;
    g.resize(n);
    gt.resize(n);
    num = n;
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        g[x - 1].pb(y - 1);
        gt[y - 1].pb(x - 1);
    }
    for (int i = 0; i < n; i++) {
        color[i] = 0;
        list[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (color[i] == 0)
            dfs1(i);
    }
    for (int i = 0; i < n; i++) {
        color[i] = 0;
    }
    num = 0;
    for (int i = 0; i < n; i++) {
        if (color[list[i]] == 0) {
            num++;
            dfs2(list[i]);
        }
    }
    cout << num << "\n";
    for (int i = 0; i < n; i++) {
        cout << color[i] << " ";
    }
    cout << "\n";
    return 0;
}
