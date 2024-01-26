#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 1e2 + 100;

bool graph[MAXN][MAXN];

deque<int> findCycle(const int N) {
    deque<int> d;
    for (int i = 0; i < N; ++i) {
        d.push_back(i);
    }

    for (int k = 0; k < N * (N - 1); ++k) {
        if (!graph[d[0]][d[1]]) {
            int i = 2;
            while (i + 1 < d.size() && (!graph[d[0]][d[i]] || !graph[d[1]][d[i + 1]])) {
                i++;
            }
            if (i + 1 == d.size()) {
                i = 2;
                while (!graph[d[0]][d[i]]) {
                    i++;
                }
            }
            reverse(d.begin() + 1, d.begin() + i + 1);
        }
        d.push_back(d.front());
        d.pop_front();
    }

    return d;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            char x;
            cin >> x;
            graph[j][i] = graph[i][j] = (x == '1');
        }
    }

    for (auto i : findCycle(n)) {
        cout << i + 1 << " ";
    }
    cout << '\n';

    return 0;
}
