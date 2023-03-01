
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

typedef struct info {
	int r, c;
	int idx;
	int num;
	int map[31][11] = { 0 };
}info;

info bridge;

int N, M, H;
bool flag = false;
int ans = 0;


bool outside(int r, int c) {
	if (r<1 || c<1 || r>H || c>N)
		return true;
	else
		return false;
}

void check() {


	int r, c;
	for (int i = 1; i <= N; i++) {
		r = 1;
		c = i;


		while (1) {
			if (bridge.map[r][c] != 0) {
				if (bridge.map[r][c] == bridge.map[r][c - 1])
					c--;

				else if (bridge.map[r][c] == bridge.map[r][c + 1])
					c++;
			}

			r++;



			if (r > H) {
				break;
			}
		}

		if (c == i) {
			if (i == N)
				flag = true;

			continue;
		}

		else
			return;
	}

}

void combi(int r, int c) {

	if (bridge.idx == bridge.num) {

		check();
		if (flag) {
			ans = bridge.num;
		}

		return;
	}

	while (bridge.map[r][c] != 0 || bridge.map[r][c + 1] != 0) {
		if (!outside(r, c + 2))
			c += 1;
		else if (!outside(r + 1, c)) {
			r += 1;
			c = 1;
		}
		else
			return;
	}

	bridge.idx++;
	bridge.map[r][c] += bridge.idx;
	bridge.map[r][c + 1] += bridge.idx;

	combi(r, c);

	if (flag)
		return;

	bridge.map[r][c] -= bridge.idx;
	bridge.map[r][c + 1] -= bridge.idx;
	bridge.idx--;

	do {
		if (!outside(r, c + 2))
			c += 1;
		else if (!outside(r + 1, c)) {
			r += 1;
			c = 1;
		}
		else
			return;

	} while (bridge.map[r][c] != 0 || bridge.map[r][c + 1] != 0);


	combi(r, c);

	return;

}

void input() {
	int r, c;
	int num;
	int cnt = -1;

	cin >> N >> M >> H;

	for (int i = 0; i < M; i++) {
		cin >> r >> c;

		bridge.map[r][c] = cnt;
		bridge.map[r][c+1] = cnt--;

	}


	for (int i = 0; i < 4; i++) {	// 사다리 i개일 때
		bridge.num = i;
		bridge.r = 1;
		bridge.c = 1;
		bridge.idx = 0;
		combi(1,1);

		if (flag)
			break;
	}

	if (bridge.num == 3 && flag == false)
		ans = -1;

	return;
}

int main() {
	input();

	cout << ans << endl;
}
