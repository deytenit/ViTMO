#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
namespace rs = ranges;

string lcs(const string &s1, const string &s2) {
  int n1 = s1.size();
  int n2 = s2.size();

  vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));

  for (int i = 1; i <= n1; ++i) {
    for (int j = 1; j <= n2; ++j) {
      if (s1[i - 1] == s2[j - 1]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
      } else {
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
      }
    }
  }

  string res;
  int i = n1, j = n2;

  while (i > 0 && j > 0) {
    if (s1[i - 1] == s2[j - 1]) {
      res += s1[i - 1];
      --i; --j;
    } else if (dp[i - 1][j] > dp[i][j - 1]) {
      --i;
    } else {
      --j;
    }
  }

  rs::reverse(res);
  return res;
}

int main() {
  string s1, s2;
  cin >> s1 >> s2;

  string res = lcs(s1, s2);

  cout << res << '\n';

  return 0;
}

