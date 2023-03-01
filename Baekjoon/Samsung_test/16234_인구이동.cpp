#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <cmath>

using namespace std;

int N, L, R;
int cnt;
int ans = 0;

int map[50][50] = { 0 };
int check[50][50] = { -1 };

int dr[4] = { 0,0,-1,1 };
int dc[4] = { 1,-1,0,0 };

queue <pair<int, int>> q_bfs;
vector <pair<int, int>> v_check[1250];
int sum[1250] = { 0 };

bool outside(int r, int c) {
	if (r < 0 || c < 0 || r >= N || c >= N)
		return true;

	else
		return false;
}

void check_move() {
	int r = q_bfs.front().first;
	int c = q_bfs.front().second;
	q_bfs.pop();
	v_check[cnt].push_back(make_pair(r, c));
	sum[cnt] += map[r][c];

	for (int i = 0; i < 4; i++) {
		int r_ = r + dr[i];
		int c_ = c + dc[i];
		if (!outside(r_, c_) && check[r_][c_] == -1) {
			if (abs(map[r_][c_] - map[r][c]) <= R && abs(map[r_][c_] - map[r][c]) >= L) {
				check[r_][c_] = cnt;
				q_bfs.push(make_pair(r_, c_));
			}
		}
	}
}

bool can_move() {
	bool flag = false;	// can move
	cnt = 0;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			flag = false;
			if (check[r][c] == -1) {

				for (int i = 0; i < 4; i++) {
					int r_ = r + dr[i];
					int c_ = c + dc[i];
					if (!outside(r_,c_) && check[r_][c_] == -1) {
						if (abs(map[r_][c_] - map[r][c]) <= R && abs(map[r_][c_] - map[r][c]) >= L) {
							flag = true;
							break;
						}
					}
				}

				if (flag) {
					q_bfs.push(make_pair(r, c));
					check[r][c] = cnt;
					while (!q_bfs.empty()) {
						check_move();
					}
					if (!v_check[cnt].empty())
						cnt++;
				}
			}
		}
	}

	if (cnt == 0)
		return false;

	else
		return true;
}

void move() {
	ans++;

	for (int i = 0; i < cnt; i++) {
		int avg = sum[i] / v_check[i].size();
		while (!v_check[i].empty()) {
			map[v_check[i].back().first][v_check[i].back().second] = avg;
			v_check[i].pop_back();
		}
	}
}

void sol() {
	while (true) {
		memset(check, -1, sizeof(check));
		memset(sum, 0, sizeof(sum));

		if (can_move())
			move();
		
		else
			break;
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> L >> R;
	
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++)
		{
			cin >> map[r][c];
		}
	}

	sol();
	cout << ans << endl;
}