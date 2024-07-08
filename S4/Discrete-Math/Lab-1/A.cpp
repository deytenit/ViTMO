#include <cmath>
#include <iostream>

using namespace std;

const int MOD = 998244353;
const int MAX_N = 2010;

int p[MAX_N], q[MAX_N], res[MAX_N];

int modsum(int a, int b) {
    long long res;

    if (b < 0) {
        res = (1LL * a + b + MOD) % MOD;
    } else {
        res = (1LL * a + b) % MOD;
    }

    return static_cast<int>(res);
}

int modmul(int a, int b) { return static_cast<int>((1LL * a * b) % MOD); }

void sum(int n, int m) {
    int power = -1;
    fill(res, res + MAX_N, 0);

    for (int i = 0; i <= max(n, m); ++i) {
        res[i] = modsum(p[i], q[i]);
        if (res[i] != 0) {
            power = i;
        }
    }

    if (power == -1) {
        cout << 0 << '\n';
        return;
    }

    cout << power << '\n';
    for (int i = 0; i <= power; ++i) {
        cout << res[i] << ' ';
    }

    cout << '\n';
}

void prod(int n, int m) {
    int power = -1;
    fill(res, res + MAX_N, 0);

    for (int i = 0; i <= max(n, m) + min(n, m); ++i) {
        res[i] = 0;
        for (int j = 0; j <= i; ++j) {
            res[i] = modsum(res[i], modmul(p[j], q[i - j]));
        }

        if (res[i] != 0) {
            power = i;
        }
    }

    if (power == -1) {
        cout << 0 << '\n';
        return;
    }

    cout << power << '\n';
    for (int i = 0; i <= power; ++i) {
        cout << res[i] << ' ';
    }

    cout << '\n';
}

void divi(int n, int m) {
    fill(res, res + MAX_N, 0);

    for (int i = 0; i < 1000; ++i) {
        int subby = 0;

        for (int j = 0; j < i; ++j) {
            subby = modsum(subby, modmul(res[j], q[i - j]));
        }

        res[i] = modsum(p[i], -subby) / q[0] % MOD;

        cout << res[i] << ' ';
    }

    cout << '\n';
}

int main() {
    int n, m;

    cin >> n >> m;
    for (int i = 0; i <= n; ++i) {
        cin >> p[i];
    }
    for (int i = 0; i <= m; ++i) {
        cin >> q[i];
    }

    sum(n, m);
    prod(n, m);
    divi(n, m);

    return 0;
}
