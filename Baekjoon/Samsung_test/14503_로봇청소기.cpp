#include <iostream>

using namespace std;

typedef struct _info {
	int r, c;
	int d;
	int cnt;
}info;

int N, M;
int map[50][50] = { 0 };
int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };
info robot = { 0 };

void sol() {
	bool flag = true;
	bool run = true;
	int r, c, d;
	while (run) {
		flag = true;
		run = false;

		if (map[robot.r][robot.c] == 0) {
			map[robot.r][robot.c] = -1;
			robot.cnt++;
		}

		for (int i = 1; i <= 4; i++) {
			d = (robot.d + 4 - i) % 4;
			r = robot.r + dr[d];
			c = robot.c + dc[d];
			if (map[r][c] == 0) {
				robot.d = d;
				robot.r = r;
				robot.c = c;
				flag = false;
				run = true;
				break;
			}
		}

		if (flag) {
			r = robot.r + dr[(robot.d + 2) % 4];
			c = robot.c + dc[(robot.d + 2) % 4];
			if (map[r][c] == -1) {
				robot.d = d;
				robot.r = r;
				robot.c = c;
				run = true;
			}
		}
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M;
	cin >> robot.r >> robot.c >> robot.d;

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < M; c++) {
			cin >> map[r][c];
		}
	}

	sol();

	cout << robot.cnt << endl;
}