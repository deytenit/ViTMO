#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

const int MOD = 998244353;

inline int at(const vector<int> &data, const size_t idx) {
    return idx < data.size() ? data[idx] : 0;
}

int gcdex(int a, int b, int &x, int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    int x1, y1;
    int g = gcdex(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return g;
}

inline int reverse_element(int a) {
    int x, y;
    int g = gcdex(a, MOD, x, y);

    return (x % MOD + MOD) % MOD;
}

inline int modsum(int a, int b) {
    return static_cast<int>(((1LL * a) + b + MOD) % MOD);
}

inline int modmul(int a, int b) {
    return static_cast<int>(((1LL * a) * b + MOD) % MOD);
}

vector<int> sum(const vector<int> &a, const vector<int> &b) {
    size_t mx_k = max(a.size(), b.size()) + 1;
    vector<int> c(mx_k, 0);

    size_t k = 0;

    for (size_t i = 0; i < mx_k; ++i) {
        c[i] = modsum(at(a, i), at(b, i));

        if (c[i] != 0) {
            k = i;
        }
    }

    c.resize(k + 1);

    return c;
}

vector<int> product(const vector<int> &a, const vector<int> &b,
                    const size_t limit) {
    size_t mx_k = min(a.size() + b.size() + 1, limit);
    vector<int> c(mx_k, 0);

    size_t k = 0;

    for (size_t i = 0; i < mx_k; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            c[i] = modsum(at(c, i), modmul(at(a, j), at(b, i - j)));
        }

        if (c[i] != 0) {
            k = i;
        }
    }

    c.resize(k + 1);

    return c;
}

vector<int> division(const vector<int> &a, const vector<int> &b,
                     const size_t mx_k) {
    vector<int> c(mx_k, 0);

    for (size_t i = 0; i < mx_k; ++i) {
        int alter = 0;

        for (size_t j = 0; j < i; ++j) {
            alter = modsum(alter, modmul(at(c, j), at(b, i - j)));
        }

        c[i] = modmul(modsum(at(a, i), -alter), reverse_element(at(b, 0)));
    }

    return c;
}

void sqroot(const vector<int> &a, const size_t mx_k) {
    vector<int> b(1);
    vector<int> c(mx_k);
    b[0] = c[0] = 1;

    int numerator = 1;
    int denominator = 1;

    for (int i = 1; i < mx_k; ++i) {
        b = product(a, b, mx_k);

        numerator = modmul(numerator, modsum(1, -modmul(2, i - 1)));
        denominator = modmul(denominator, modmul(modsum(i - 1, 1), 2));

        int rev_binom = modmul(numerator, reverse_element(denominator));

        for (int j = 0; j < mx_k; ++j) {
            c[j] = modsum(c[j], modmul(at(b, j), rev_binom));
        }
    }

    for (int i : c) {
        cout << i << ' ';
    }
    cout << '\n';
}

void exponent(const vector<int> &a, const size_t mx_k) {
    vector<int> b(1);
    vector<int> c(mx_k);
    b[0] = c[0] = 1;
    int e = 1;

    for (int i = 1; i < mx_k; ++i) {
        b = product(a, b, mx_k);
        e = modmul(e, i);

        int e_rev = reverse_element(e);

        for (int j = 0; j < mx_k; ++j) {
            c[j] = modsum(c[j], modmul(at(b, j), e_rev));
        }
    }

    for (int i : c) {
        cout << i << ' ';
    }
    cout << '\n';
}

void logarythm(const vector<int> &a, const size_t mx_k) {
    vector<int> b(1);
    vector<int> c(mx_k);
    b[0] = 1;
    int l = modsum(0, -1);

    for (int i = 1; i < mx_k; ++i) {
        b = product(a, b, mx_k);
        l = modsum(0, -l);

        int l_rev = modmul(l, reverse_element(i));

        for (int j = 0; j < mx_k; ++j) {
            c[j] = modsum(c[j], modmul(at(b, j), l_rev));
        }
    }

    for (int i : c) {
        cout << i << ' ';
    }
    cout << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;

    vector<int> p(n + 1);
    for (int i = 0; i <= n; ++i) {
        cin >> p[i];
    }

    sqroot(p, m);

    exponent(p, m);

    logarythm(p, m);

    return 0;
}
