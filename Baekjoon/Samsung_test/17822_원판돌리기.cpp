#include <iostream>
#include <vector>
#include <deque>

#define CW 0
#define CCW 1

using namespace std;

deque <int> dq[50];

typedef struct info_ {
	int x, d, k;
}info;

vector <info> testcase;
int N, M, T; // N 원판개수, M 칸, T 번 회전
int dr[4] = { 0,1 };
int dc[4] = { 1,0 };

bool outside(int i, int j) {
	if (i < 0 || j < 0 || i >= N || j >= M)
		return true;
	else
		return false;
}

void erase() {
	int check[50][50] = { 0 };

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {

			if (dq[i][j] != 0) {
				if (!outside(i+1,j) && dq[i][j] == dq[i + 1][j])
					check[i][j] = check[i + 1][j] = 1;

				if (!outside(i,(j+1)%M) && dq[i][j] == dq[i][(j+1)%M])
					check[i][j] = check[i][(j+1)%M] = 1;
			}
		}
	}

	bool flag = true;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (check[i][j] == 1) {
				dq[i][j] = 0;
				flag = false;
			}
		}
	}

	// average
	if (flag) {
		double avg = 0;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (dq[i][j] != 0) {
					avg += dq[i][j];
					cnt++;
				}
			}
		}
		avg = double((double)avg / cnt);
		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (dq[i][j] != 0) {
					if (avg < dq[i][j]) dq[i][j]--;
					else if (avg > dq[i][j]) dq[i][j]++;
				}
			}
		}
	}

}

void rotate(int t) {
	for (int n = 1; n < N; n++) {
		if ((n + 1) % testcase[t].x == 0) {
			// CW
			if (testcase[t].d == CW) {
				for (int k = 0; k < testcase[t].k; k++) {
					dq[n].push_front(dq[n].back());
					dq[n].pop_back();
				}
			}

			//CCW
			else {
				for (int k = 0; k < testcase[t].k; k++) {
					dq[n].push_back(dq[n].front());
					dq[n].pop_front();
				}
			}
		}
	}
}

void sol() {
	for (int t = 0; t < testcase.size(); t++) {
		rotate(t);
		erase();
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M >> T;

	for (int n = 0; n < N; n++) {
		for (int m = 0; m < M; m++) {
			int tmp;
			cin >> tmp;
			dq[n].push_back(tmp);
		}
	}

	for (int t = 0; t < T; t++) {
		info tmp;
		cin >> tmp.x >> tmp.d >> tmp.k;
		if (tmp.k > M / 2) {
			tmp.d ^= 1;
			tmp.k = M - tmp.k;
		}
		testcase.push_back(tmp);
	}
	
	sol();

	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			ans += dq[i][j];
		}
	}
	cout << ans << endl;
}