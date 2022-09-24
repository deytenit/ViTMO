#include <cmath>
#include <iostream>

using namespace std;

int a[100000];

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int q;
    cin >> q;
    int diff = abs(q - a[0]);
    int ans = a[0];
    for (int i = 1; i < n; i++) {
        if (abs(q - a[i]) < diff) {
            diff = abs(q - a[i]);
            ans = a[i];
        }
    }
    cout << ans << endl;
    return 0;
}