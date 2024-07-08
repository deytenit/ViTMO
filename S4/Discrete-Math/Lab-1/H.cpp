#include <iostream>
#include <vector>

using namespace std;

const int MOD = 998244353;
const int MAXN = 5010;

int C[MAXN][MAXN];

inline int at(const vector<int> &v, int i) { return i < v.size() ? v[i] : 0; }

inline int modsum(int a, int b) {
    return static_cast<int>((1LL * a + b + MOD) % MOD);
}

inline int modmul(int a, int b) {
    return static_cast<int>((1LL * a * b + MOD) % MOD);
}

vector<int> division(const vector<int> &a, const vector<int> &b,
                     const int limit) {
    vector<int> res(limit, 0);

    for (int i = 0; i < limit; ++i) {
        int subby = 0;
        for (int j = 0; j < i; ++j) {
            subby = modsum(subby, modmul(at(res, j), at(b, i - j)));
        }

        res[i] = modsum(at(a, i), -subby) / at(b, 0);
    }

    return res;
}

inline int sgn(int x) { return (x & 1) ? -1 : 1; }

void generate_triangle(const int maxn) {
    C[0][0] = 1;
    for (int n = 1; n <= maxn; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            C[n][k] = modsum(C[n - 1][k - 1], C[n - 1][k]);
        }
    }
}

int main() {
    int k, n;
    cin >> k >> n;

    generate_triangle(n);

    vector<int> p((k - 2) / 2 + 1);
    for (int i = 0; i < p.size(); ++i) {
        p[i] = modmul(sgn(i), C[k - i - 2][i]);
    }

    vector<int> q((k - 1) / 2 + 1);
    for (int i = 0; i < q.size(); ++i) {
        q[i] = modmul(sgn(i), C[k - i - 1][i]);
    }

    auto res = division(p, q, n);
    for (int i = 0; i < n; ++i) {
        cout << res[i] << '\n';
    }

    return 0;
}
