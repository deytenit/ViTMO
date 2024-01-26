#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
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
const int MAXN = 1e3 + 100;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<int> v(n);
    iota(v.begin(), v.end(), 1);

    stable_sort(v.begin(), v.end(), [](int a, int b) {
        cout << 1 << ' ' << a << ' ' << b << endl;
        string response;
        cin >> response;
        return response == "YES";
    });

    cout << 0 << ' ';
    for (auto i : v) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}
