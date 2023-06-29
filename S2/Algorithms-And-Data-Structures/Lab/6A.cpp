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

ll n, l;
vector<vector<ll> > g;
vector<ll> tin, tout;
ll timer;
vector<vector<ll> > up;
vector<ll> a;

void dfs(ll v, ll p = 0) {
    tin[v] = ++timer;
    up[v][0] = p;
    for (ll i = 1; i <= l; i++)
        up[v][i] = up[up[v][i - 1]][i - 1];
    for (ll i = 0; i < g[v].size(); i++) {
        ll to = g[v][i];
        if (to != p)
            dfs(to, v);
    }
    tout[v] = ++timer;
}

bool upper(ll a, ll b) {
    return tin[a] <= tin[b] && tout[a] >= tout[b];
}

ll lca(ll a, ll b) {
    if (upper(a, b))
        return a;
    if (upper(b, a))
        return b;
    for (ll i = l; i >= 0; i--)
        if (!upper(up[a][i], b))
            a = up[a][i];
    return up[a][0];
}

int main() {
    ll m;
    cin >> n >> m;
    tin.resize(n);
    tout.resize(n);
    up.resize(n);
    g.resize(n);
    a.resize(2 * m + 1);
    for (ll i = 1; i < n; i++) {
        ll x;
        cin >> x;
        g[x].pb(i);
        g[i].pb(x);
    }
    l = 1;
    while ((1 << l) <= n)
        l++;
    for (ll i = 0; i < n; i++)
        up[i].resize(l + 1);
    dfs(0);
    cin >> a[0] >> a[1];
    ll x, y, z;
    cin >> x >> y >> z;
    for (ll i = 2; i <= 2 * m; i++) {
        a[i] = (x * a[i - 2] + y * a[i - 1] + z) % n;
    }
    ll v = 0;
    ll res = 0;
    for (ll i = 0; i < m; i++) {
        v = lca((a[2 * i] + v) % n, a[2 * i + 1]);
        res += v;
    }
    cout << res << '\n';
    return 0;
}