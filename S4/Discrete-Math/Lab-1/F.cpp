#include <iostream>
#include <vector>

using namespace std;

const int MOD = 1e9 + 7;
const int INF = 0x7fffffff;

inline int at(const vector<int> &data, const size_t idx) {
    return idx < data.size() ? data[idx] : 0;
}

inline int modsum(int a, int b) {
    return static_cast<int>(((1LL * a) + b + MOD) % MOD);
}

inline int modmul(int a, int b) {
    return static_cast<int>(((1LL * a) * b + MOD) % MOD);
}

int ctln(const vector<int> &a, const int n) {
    int res = 0;

    for (int i = 0; i <= n; ++i) {
        res = modsum(res, modmul(a[i], a[n - i]));
    }

    return res;
}

int main() {
    int k, m;
    cin >> k >> m;

    vector<int> c(k);
    for (auto &i : c) {
        cin >> i;
    }

    vector<int> trees(m + 1, 0);
    vector<int> seqtrees(m + 1, 0);
    trees[0] = seqtrees[0] = 1;

    for (int i = 1; i <= m; ++i) {
        for (auto w : c) {
            if (i < w) {
                break;
            }

            trees[i] = modsum(trees[i], seqtrees[i - w]);
        }

        seqtrees[i] = ctln(trees, i);

        cout << trees[i] << ' ';
    }
    cout << '\n';
}
