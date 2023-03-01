#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

int ans = 0;
int N, L;
int map[100][100] = { 0 };
int load[2][100] = { 0 };

bool outside(int i) {
	if (i < 0 || i >= N)
		return true;
	else
		return false;
}

bool construct(int idx) {
	bool tf;

	if (load[1][idx] == 1) {
		return false;
	}

	else {
		load[1][idx] = 1;
		tf = true;
	}

	if (L == 1) return tf;

	for (int i = idx; i < idx + L - 1; i++) {
		if (outside(i + 1) || load[1][i + 1] == 1)
			return false;

		else {
			if (load[0][i] == load[0][i + 1]) {
				load[1][i + 1] = 1;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

void solve() {
	int i, j;
	for (i = 0; i < N; i++) {
		//initialize
		for (int j = 0; j < N; j++) {
			load[0][j] = map[i][j];
		}
		memset(load[1], 0, sizeof(load[1]));
		
		for (j = 0; j < N-1; j++) {
			
			if (abs(load[0][j] - load[0][j + 1]) > 1)
				break;

			if (load[0][j] < load[0][j + 1]) {
				if (!construct(j - L + 1))
					break;
			}
			else if (load[0][j] > load[0][j + 1]) {
				if (!construct(j + 1))
					break;
			}
		}

		if (j == N - 1) {
			ans++;
		}
	}
	
	for (i = 0; i < N; i++) {
		//initialize
		for (int j = 0; j < N; j++) {
			load[0][j] = map[j][i];
		}
		memset(load[1], 0, sizeof(load[1]));


		for (j = 0; j < N - 1; j++) {

			if (abs(load[0][j] - load[0][j + 1]) > 1)
				break;

			if (load[0][j] < load[0][j + 1]) {
				if (!construct(j - L + 1))
					break;
			}
			else if (load[0][j] > load[0][j + 1]) {
				if (!construct(j + 1))
					break;
			}
		}

		if (j == N - 1) {
			ans++;
		}
	}
}

void input_() {
	cin >> N >> L;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	input_();

	solve();

	cout << ans << endl;

}