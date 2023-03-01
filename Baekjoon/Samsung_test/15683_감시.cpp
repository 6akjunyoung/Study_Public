#include <iostream>
#include <vector>

using namespace std;

int N, M;
int map[8][8] = { 0 };
int visit[8][8] = { 0 };
int ans = 64;
vector <pair<int, int>> cctv;

int dr[4] = { 0,1,0,-1 };
int dc[4] = { 1,0,-1,0 };

bool outside(int r, int c) {
	if (r < 0 || r >= N || c < 0 || c >= M)
		return true;
	else
		return false;
}

void go(int r_, int c_, int dir, int pm) {
	int r = r_ + dr[dir];
	int c = c_ + dc[dir];

	while (true) {
		if (!outside(r, c) && map[r][c] != 6) {
			visit[r][c] += pm;
			r += dr[dir];
			c += dc[dir];
		}
		else
			break;
	}
}

void solution(int idx) {
	if (ans == 0) return;
	if (cctv.size() == idx) {
		int ans_ = 0;
		for (int r = 0; r < N; r++) {
			for (int c = 0; c < M; c++) {
				if (visit[r][c] == 0)
					ans_++;
			}
		}
		ans = ans < ans_ ? ans : ans_;
		return;
	}

	int r = cctv[idx].first;
	int c = cctv[idx].second;

	for (int dir = 0; dir < 4; dir++) {
		if (ans == 0) return;
		switch (map[r][c]) {
		case 1:
			go(r, c, dir, 1);
			solution(idx + 1);
			go(r, c, dir, -1);
			break;
		case 2:
			if (dir == 3 || dir == 4) break;
			go(r, c, dir, 1);
			go(r, c, (dir + 2) % 4, 1);
			solution(idx + 1);
			go(r, c, dir, -1);
			go(r, c, (dir + 2) % 4, -1);
			break;
		case 3:
			go(r, c, dir, 1);
			go(r, c, (dir + 1) % 4, 1);
			solution(idx + 1);
			go(r, c, dir, -1);
			go(r, c, (dir + 1) % 4, -1);
			break;
		case 4:
			go(r, c, dir, 1);
			go(r, c, (dir + 1) % 4, 1);
			go(r, c, (dir + 2) % 4, 1);
			solution(idx + 1);
			go(r, c, dir, -1);
			go(r, c, (dir + 1) % 4, -1);
			go(r, c, (dir + 2) % 4, -1);
			break;
		}
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			cin >> map[r][c];

			if (map[r][c] != 0) {
				visit[r][c] += 1;
				if (map[r][c] > 0 && map[r][c] < 5) {
					cctv.push_back(make_pair(r, c));
				}
			}
		}
	}

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			if (map[r][c] == 5) {
				for (int dir = 0; dir < 4; dir++) {
					go(r, c, dir, 1);
				}
			}
		}
	}

	solution(0);

	cout << ans << endl;
}