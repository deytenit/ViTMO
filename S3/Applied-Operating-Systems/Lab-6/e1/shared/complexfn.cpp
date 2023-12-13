#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace std;

using ll = long long;
using matrix = vector<vector<ll>>;

const int N = 20;
const int M = 1000000007;

matrix matmul(const matrix& x, const matrix& y) {
	matrix res(N, vector<ll>(N, 0));

	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			for (size_t k = 0; k < N; ++k) {
				res[i][j] = (res[i][j] + (x[i][k] * y[k][j]) % M) % M;
			}
		}
	}

	return res;
}

matrix pow(const matrix& a, const ll power) {
	matrix res(N, vector<ll>(N, 0));
	for (size_t i = 0; i < N; ++i) {
		res[i][i] = 1;
	}

	for (size_t i = 0; i < power; ++i) {
		res = matmul(res, a);
	}

	return res;
}

int main(int argc, char **argv) {
	matrix mtrx(N, vector<ll>(N, 0));
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, stoi(static_cast<string>(argv[1])));

	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			mtrx[i][j] = distrib(gen); 
		}
	}

	matrix res = pow(mtrx, 25000);

	ll sum = 0;

	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			sum = (sum + res[i][j]) % M;
		}
	}

	cout << sum << '\n';
	return 0;
}

