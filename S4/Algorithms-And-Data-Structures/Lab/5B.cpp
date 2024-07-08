#include <cmath>
#include <iostream>

using namespace std;

#define fast()                                                                 \
  ios_base::sync_with_stdio(false);                                            \
  cin.tie(0);                                                                  \
  cout.tie(0);

#define fstr()                                                                 \
  ifstream cin("input.txt");                                                   \
  ofstream cout("output.txt");

#define pb push_back
#define sqr(a) ((a) * (a))
#define fp first
#define sp second
#define mp make_pair

typedef long long ll;
typedef long double ld;

const int INF = 2e9;
const ll MOD = 1e9 + 23;
const ld EPS = 1e-10;
const ld PI = 3.14159265358979323;
const ll P = 69;

struct Point {
  long double x;
  long double y;
  Point() : x(0), y(0){};
  Point(ld _x, ld _y) : x(_x), y(_y){};
  friend istream &operator>>(istream &in, Point &p) {
    in >> p.x >> p.y;
    return in;
  }
  friend ostream &operator<<(ostream &out, Point &p) {
    out << "P(" << p.x << ";" << p.y << ")";
    return out;
  }
};

struct Vector {
  long double x;
  long double y;
  Vector() : x(0), y(0){};
  Vector(ld _x, ld _y) : x(_x), y(_y){};
  Vector(const Point &A, const Point &B) {
    x = B.x - A.x;
    y = B.y - A.y;
  }
  long double length() { return hypotl(x, y); }
  Vector operator+(const Vector &v) { return Vector(x + v.x, y + v.y); }
  Vector operator-(const Vector &v) { return Vector(x - v.x, y - v.y); }
  friend Vector operator*(long double k, const Vector &v) {
    return Vector(k * v.x, k * v.y);
  }
  Vector norm() {
    long double leng = length();
    return Vector(x / leng, y / leng);
  }
  Vector normal() { return Vector(-y, x); }
  long double operator*(const Vector &v) { return x * v.x + y * v.y; }
  long double operator%(const Vector &v) { return x * v.y - y * v.x; }
};

int main() {
  fast();
  Point A, O, B, P;
  cin >> A >> O >> B >> P;
  Vector OA(O, A);
  Vector OP(O, P);
  Vector OB(O, B);
  ld prd1 = OA % OP;
  ld prd2 = OP % OB;
  ld prd3 = OA % OB;
  if ((prd1 >= 0 && prd2 >= 0 && prd3 >= 0) ||
      (prd1 <= 0 && prd2 <= 0 && prd3 <= 0)) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
  }
  return 0;
}
