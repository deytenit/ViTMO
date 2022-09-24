#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

int main() {
    int n;
    cin >> n;

    int mn1 = 2000000001, mn2 = 2000000001;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        mn2 = min(mn2, x);
        if (mn1 > mn2) {
            swap(mn1, mn2);
        }
    }

    cout << mn1 << ' ' << mn2 << '\n';
    return 0;
}