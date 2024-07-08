#include <cmath>
#include <iostream>

using namespace std;

#define int long long

const int MAX_N = 2010;

int a[MAX_N], c[MAX_N], q[MAX_N], p[MAX_N];

int prod(int k) {
    int power = -1;
    fill(p, p + MAX_N, 0);

    for (int i = 0; i <= k; ++i) {
        p[i] = 0;
        for (int j = 0; j <= i; ++j) {
            p[i] += a[j] * q[i - j];
        }

        if (p[i] != 0) {
            power = i;
        }
    }

    return power;
}

signed main() {
    int k;
    cin >> k;

    for (int i = 0; i < k; ++i) {
        cin >> a[i];
    }

    q[0] = 1;
    for (int i = 0; i < k; ++i) {
        cin >> c[i];
        q[i + 1] = -c[i];
    }

    int ppower = prod(k - 1);

    cout << ppower << '\n';
    for (int i = 0; i <= ppower; ++i) {
        cout << p[i] << ' ';
    }
    cout << '\n';

    cout << k << '\n';
    for (int i = 0; i <= k; ++i) {
        cout << q[i] << ' ';
    }
    cout << '\n';

    return 0;
}
