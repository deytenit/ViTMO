#include <algorithm>
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

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
const int MAXN = 5e5 + 1000;

struct hash_map {
    using bucket = list<int>;

    vector<bucket> data{MAXN};

    void insert(int value) {
        bucket& buck = data[hash_value(value)];

        if (find(buck.begin(), buck.end(), value) == buck.end()) {
            buck.push_back(value);
        }
    }

    bool exists(int value) {
        bucket& buck = data[hash_value(value)];

        return find(buck.begin(), buck.end(), value) != buck.end();
    }

    void remove(int value) {
        bucket& buck = data[hash_value(value)];

        auto it = find(buck.begin(), buck.end(), value);

        if (it != buck.end()) {
            buck.erase(it);
        }
    }

    size_t hash_value(int value) {
        return std::hash<int>{}(value) % data.size();
    }
};

int main() {
#ifdef _LOCAL
    auto rtime = high_resolution_clock::now();
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    fast();

    hash_map mp;

    string s;
    int x;

    while (cin >> s && cin >> x) {
        if (s[0] == 'i') {
            mp.insert(x);
            continue;
        }
        if (s[0] == 'd') {
            mp.remove(x);
            continue;
        }
        if (s[0] == 'e') {
            cout << (mp.exists(x) ? "true" : "false") << '\n';
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}