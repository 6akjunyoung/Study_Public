#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

vector <int> v[11][11];
int A[11][11] = { 5, };
int A_p[11][11] = { 0 };
int N, M, K;

int dx[8] = { 0,0,1,1,1,-1,-1,-1 };
int dy[8] = { 1,-1,1,0,-1,1,0,-1};

bool outside(int r, int c) {
	if (r<1 || c <1 || r>N || c>N) {
		return true;
	}
	else
		return false;
}

void winter() {
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= N; j++) {
			A[i][j] += A_p[i][j];
		}
	}
}

void fall() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			
			for (int k = 0; k < v[i][j].size(); k++) {
				if (v[i][j][k] % 5 == 0) {
					for (int l = 0; l < 8; l++) {
						if (!outside(i + dx[l], j + dy[l])) {
							v[i + dx[l]][j + dy[l]].push_back(1);
						}
					}
				}
			}

		}
	}
}

void spring() {

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (v[i][j].size() != 0) {
				
				sort(v[i][j].begin(), v[i][j].end());

				// 양분먹기
				for (int k = 0; k < v[i][j].size(); k++) {
					
					if (v[i][j][k] <= A[i][j]) {
						A[i][j] -= v[i][j][k];
						v[i][j][k]++;
					}

					else {

						while (k < v[i][j].size()) {

							A[i][j] += ( v[i][j].back() )/2;
							v[i][j].pop_back();
						}

						break;
					}
					
				}

			}
		}
	}
}

void solve() {
	// 봄
	// 여름
	spring();

	// 가을 
	fall();

	//겨울
	winter();
}

void input() {
	int age;
	int r, c;

	cin >> N >> M >> K;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> A_p[i][j];

			A[i][j] = 5;
		}
	}

	for (int i = 0; i < M; i++) {
		cin >> r >> c >> age;
		v[r][c].push_back(age);
	}
	
}
int main() {
	
	int ans = 0;
	
	input();
		
	for (int i = 0; i < K; i++) {
		solve();
	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			ans += v[i][j].size();
			
		}
	}

	cout << ans << endl;
}