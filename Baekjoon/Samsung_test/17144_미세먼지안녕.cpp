#include <iostream>
#include <cstring>
#include <cstdio>
#include <queue>

using namespace std;

int map[51][51] = { 0 };
int mp[51][51] = { 0 };

int air[2][2] = { 0 };

int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1, 0,-1 };

int R, C, T;

bool out(int r, int c) {	// 외곽, 공기청정기
	if (r<1 || c<1 || r>R || c>C || ((r == air[0][0]) && (c == air[0][1])) || ((r == air[1][0]) && (c == air[1][1]))) {
		return true;
	}

	else
		return false;

}

void fresh() {

	int r, c;
	int i;
	for (int a = 0; a < 2; a++) {
		r = air[a][0];
		c = air[a][1];

		if (a == 0) {
			
			map[r-1][c] = 0;
			
			for (i = 0; i < 4; i++) {
				while (1) {
					
					r += dx[i];
					c += dy[i];

					map[r - dx[i]][c - dy[i]] = map[r][c];

					if (out(r+dx[i],c+dy[i]) || r+dx[i] == air[1][0])
						break;
				}
			}
			map[air[0][0]][air[0][1] + 1] = 0;
		}

		else {
			map[r+1][c] = 0;
			i = 2;

			do {
				while (1) {

					r += dx[i];
					c += dy[i];

					map[r - dx[i]][c - dy[i]] = map[r][c];

					if (out(r + dx[i], c + dy[i]) || r + dx[i] == air[0][0])
						break;
				}
				i = (i - 1 + 4) % 4;

			} while (i!=2);
			map[air[1][0]][air[1][1] + 1] = 0;
		}
	}

	
}

void diff() {

	int plus = 0;
	int nx = 0;
	int ny = 0;
	

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {

			plus = map[i][j] / 5;
			if ( plus > 0 ) {

				for (int k = 0; k < 4; k++) {
					nx = i + dx[k];
					ny = j + dy[k];

					if (!out(nx,ny)) {
						mp[nx][ny] += plus;
						mp[i][j] -= plus;
					}

				}
				
			}

		}
	}

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
	
			map[i][j] += mp[i][j];

		}
	}

	memset(mp, 0, sizeof(mp));
}

void solve() {
	
	diff();

	fresh();

}

void input() {
	cin >> R >> C >> T;

	int a = 0;

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			cin >> map[i][j];
			
			if (map[i][j] == -1) {
				air[a][0] = i;
				air[a][1] = j;
				a++;
				map[i][j] = 0;
			}
		}
	}

}

int main() {
	input();

	while (T--) {
		solve();
	}

	int ans = 0;

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			ans += map[i][j];
		}
	}

	cout << ans << endl;
}