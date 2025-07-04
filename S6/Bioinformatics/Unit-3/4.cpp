#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
namespace rs = ranges;

const int GAP_PENALTY = -2;

pair<string, string> diff(const string& s1, const string& s2) {
  int n1 = s1.size();
  int n2 = s2.size();

  vector<vector<int>> dp(n1 + 1, vector<int>(n2 + 1, 0));
  vector<vector<int>> traceback(n1 + 1, vector<int>(n2 + 1, 0));

  for (int i = 1; i <= n1; ++i) {
    dp[i][0] = dp[i - 1][0] + GAP_PENALTY;
    traceback[i][0] = 1;
  }
  for (int j = 1; j <= n2; ++j) {
    dp[0][j] = dp[0][j - 1] + GAP_PENALTY;
    traceback[0][j] = 2;
  }

  for (int i = 1; i <= n1; ++i){
    for (int j = 1; j <= n2; ++j){
      int diag = dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 2 : -1);
      int up = dp[i - 1][j] + GAP_PENALTY;
      int left = dp[i][j - 1] + GAP_PENALTY;

      dp[i][j] = max({diag, up, left});

      if (dp[i][j] == diag) {
        traceback[i][j] = 0;
      } else if (dp[i][j] == up) {
        traceback[i][j] = 1;
      } else {
        traceback[i][j] = 2;
      }
    }
  }

  int i = n1, j = n2;
  string diff1, diff2;

  while (i > 0 || j > 0){
    if (i > 0 && j > 0 && traceback[i][j] == 0) {
      diff1 += s1[i - 1];
      diff2 += s2[j - 1];
      i--; j--;
    } else if (i > 0 && traceback[i][j] == 1) {
      diff1 += s1[i - 1];
      diff2 += '-';
      i--;
    } else {
      diff1 += '-';
      diff2 += s2[j - 1];
      j--;
    }
  }

  rs::reverse(diff1);
  rs::reverse(diff2);
  return {diff1, diff2};
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;

    auto [diff1, diff2] = diff(s1, s2);

    cout << diff1 << '\n';
    cout << diff2 << '\n';

    return 0;
}
