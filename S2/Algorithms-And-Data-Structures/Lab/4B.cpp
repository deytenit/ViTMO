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

using ll = long long;
using ld = long double;

const int INF = 0x7fffffff;
const ll MOD = 1e9 + 7;
const ld EPS = 1e-8;
const ld PI = 3.14159265358979323;
constexpr int MAXN = 2e5 + 1000;

struct hash_map {
    using storage = list<pair<string, string>>;
    using bucket = list<storage::iterator>;

    vector<bucket> data{MAXN};
    storage link;

    bucket& get_bucket(const string& value) {
        return data[hash<string>{}(value) % data.size()];
    }

    void insert(const string& key, const string& value) {
        bucket& bkt = get_bucket(key);

        auto it = find_if(bkt.begin(), bkt.end(), [&key](const auto& it) { return it->first == key; });

        if (it == bkt.end()) {
            link.emplace_back(key, value);
            bkt.emplace_back(prev(link.end()));
        } else {
            (*it)->second = value;
        }
    }

    string get(const string& key) {
        bucket& bkt = get_bucket(key);

        auto it = find_if(bkt.begin(), bkt.end(), [&key](const auto& it) { return it->first == key; });

        if (it == bkt.end()) {
            return "none";
        }

        return (*it)->second;
    }

    void remove(const string& key) {
        bucket& bkt = get_bucket(key);

        auto it = find_if(bkt.begin(), bkt.end(), [&key](const auto& it) { return it->first == key; });

        if (it == bkt.end()) {
            return;
        }

        link.erase(*it);
        bkt.erase(it);
    }

    string get_next(const string& key) {
        bucket& bkt = get_bucket(key);

        auto it = find_if(bkt.begin(), bkt.end(), [&key](const auto& it) { return it->first == key; });

        if (it == bkt.end() || *it == link.end() || *it == prev(link.end())) {
            return "none";
        }

        return next(*it)->second;
    }

    string get_prev(const string& key) {
        bucket& bkt = get_bucket(key);

        auto it = find_if(bkt.begin(), bkt.end(), [&key](const auto& it) { return it->first == key; });

        if (it == bkt.end() || *it == link.begin()) {
            return "none";
        }

        return prev(*it)->second;
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

    string s, x, y;

    while (cin >> s && cin >> x) {
        if (s == "put") {
            cin >> y;
            mp.insert(x, y);
            continue;
        }
        if (s[0] == 'd') {
            mp.remove(x);
            continue;
        }
        if (s[0] == 'g') {
            cout << mp.get(x) << '\n';
            continue;
        }
        if (s == "prev") {
            cout << mp.get_prev(x) << '\n';
            continue;
        }
        if (s[0] == 'n') {
            cout << mp.get_next(x) << '\n';
        }
    }

#ifdef _LOCAL
    duration<double> diff = high_resolution_clock::now() - rtime;
    cerr << "Runtime: " << diff.count() << " s.\n";
#endif
    return 0;
}