#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;
using namespace chrono;

#define fast()                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

#define ff first
#define ss second
#define sqr(a) ((a) * (a))
#define vout(a) cout << #a << ": " << (a) << '\n'
#define fsqrt(a) static_cast<size_t>(sqrt(a))

using ll = long long;
using ld = long double;

constexpr int INF = 0x7fffffff;
constexpr ll MOD = 1e9 + 7;
constexpr ld EPS = 1e-8;
constexpr ld PI = 3.14159265358979323;
constexpr int MAXN = 2e5 + 1000;

vector<vector<int>> decomp;
vector<int> mxs;

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    int n, m;
    cin >> n >> m;

    const int NSQRT = fsqrt(n) + 1;

    for (int i = 0; i < n; ++i) {
        if (i % NSQRT == 0) {
            decomp.emplace_back();
            mxs.emplace_back(-INF);
        }

        int x;
        cin >> x;
        decomp.back().push_back(x);
        mxs.back() = max(mxs.back(), x);
    }

    while (m--) {
        int cmd, k, x;
        cin >> cmd >> k >> x;
        --k;
        switch (cmd) {
            case 1: {
                int idx = k / NSQRT;

                if (mxs[idx] >= x) {
                    auto it = find_if(decomp[idx].begin() + (k % NSQRT), decomp[idx].end(), [x](int y) { return y >= x; });
                    if (it != decomp[idx].end()) {
                        cout << idx * NSQRT + distance(decomp[idx].begin(), it) + 1 << '\n';
                        break;
                    }
                }

                ++idx;
                while (idx < mxs.size() && mxs[idx] < x) {
                    ++idx;
                }

                if (idx != mxs.size()) {
                    auto it = find_if(decomp[idx].begin(), decomp[idx].end(), [x](int y) { return y >= x; });
                    cout << idx * NSQRT + distance(decomp[idx].begin(), it) + 1 << '\n';
                    break;
                }

                cout << -1 << '\n';

                break;
            }
            case 0: {
                int idx = k / NSQRT;

                decomp[idx][k % NSQRT] = x;
                mxs[idx] = *max_element(decomp[idx].begin(), decomp[idx].end());

                break;
            }
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}