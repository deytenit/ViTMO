#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> Z_function(const string& s) {
    vector<int> Z(s.size());
    int l = 0, r = 0;

    for (int i = 1; i < s.size(); ++i) {
        if (i < r) {
            Z[i] = min(r - i, Z[i - l]);
        }
        while (i + Z[i] < s.size() && s[Z[i]] == s[i + Z[i]]) {
            Z[i]++;
        }
        if (i + Z[i] > r) {
            l = i;
            r = i + Z[i];
        }
    }

    Z[0] = s.size();

    return Z;
}

int main() {
    string s;
    cin >> s;

    for (auto i : Z_function(s)) {
        cout << i << ' ';
    }
    cout << '\n';

    return 0;
}
