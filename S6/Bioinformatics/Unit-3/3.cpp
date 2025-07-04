#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
namespace rs = ranges;

const int GAP_PENALTY = -5;

const int blosum62[20][20] = {
    {  4,  0, -2, -1, -2,  0, -2, -1, -1, -1, -1, -2, -1, -1, -1,  1,  0,  0, -3, -2 },
    {  0,  9, -3, -4, -2, -3, -3, -1, -3, -1, -1, -3, -3, -3, -3, -1, -1, -1, -2, -2 },
    { -2, -3,  6,  2, -3, -1, -1, -3, -1, -4, -3,  1, -1,  0, -2,  0, -1, -3, -4, -3 },
    { -1, -4,  2,  5, -3, -2,  0, -3,  1, -3, -2,  0, -1,  2,  0,  0, -1, -2, -3, -2 },
    { -2, -2, -3, -3,  6, -3, -1,  0, -3,  0,  0, -3, -4, -3, -3, -2, -2, -1,  1,  3 },
    {  0, -3, -1, -2, -3,  6, -2, -4, -2, -4, -3,  0, -2, -2, -2,  0, -2, -3, -2, -3 },
    { -2, -3, -1,  0, -1, -2,  8, -3, -1, -3, -2,  1, -2,  0,  0, -1, -2, -3, -2,  2 },
    { -1, -1, -3, -3,  0, -4, -3,  4, -3,  2,  1, -3, -3, -3, -3, -2, -1,  3, -3, -1 },
    { -1, -3, -1,  1, -3, -2, -1, -3,  5, -2, -1,  0, -1,  1,  2,  0, -1, -2, -3, -2 },
    { -1, -1, -4, -3,  0, -4, -3,  2, -2,  4,  2, -3, -3, -2, -2, -2, -1,  1, -2, -1 },
    { -1, -1, -3, -2,  0, -3, -2,  1, -1,  2,  5, -2, -2,  0, -1, -1, -1,  1, -1, -1 },
    { -2, -3,  1,  0, -3,  0,  1, -3,  0, -3, -2,  6, -2,  0,  0,  1,  0, -3, -4, -2 },
    { -1, -3, -1, -1, -4, -2, -2, -3, -1, -3, -2, -2,  7, -1, -2, -1, -1, -2, -4, -3 },
    { -1, -3,  0,  2, -3, -2,  0, -3,  1, -2,  0,  0, -1,  5,  1,  0, -1, -2, -2, -1 },
    { -1, -3, -2,  0, -3, -2,  0, -3,  2, -2, -1,  0, -2,  1,  5, -1, -1, -3, -3, -2 },
    {  1, -1,  0,  0, -2,  0, -1, -2,  0, -2, -1,  1, -1,  0, -1,  4,  1, -2, -3, -2 },
    {  0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1,  0, -1, -1, -1,  1,  5,  0, -2, -2 },
    {  0, -1, -3, -2, -1, -3, -3,  3, -2,  1,  1, -3, -2, -2, -3, -2,  0,  4, -3, -1 },
    { -3, -2, -4, -3,  1, -2, -2, -3, -3, -2, -1, -4, -4, -2, -3, -3, -2, -3, 11,  2 },
    { -2, -2, -3, -2,  3, -3,  2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1,  2,  7 }
};

const string amino = "ACDEFGHIKLMNPQRSTVWY";

int index[26];

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
      int diag = dp[i - 1][j - 1] + blosum62[index[s1[i - 1] - 'A']][index[s2[j - 1] - 'A']];
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
    for (int i = 0; i < amino.size(); ++i) {
        index[amino[i] - 'A'] = i;
    }

    string s1, s2;
    cin >> s1 >> s2;

    auto [diff1, diff2] = diff(s1, s2);

    cout << diff1 << '\n';
    cout << diff2 << '\n';

    return 0;
}
