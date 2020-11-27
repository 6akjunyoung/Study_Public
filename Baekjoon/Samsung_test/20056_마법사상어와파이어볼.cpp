#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int dr[8] = { -1,-1,0,1,1,1,0,-1 };
int dc[8] = { 0,1,1,1,0,-1,-1,-1 };

typedef struct info_ {
	int m, d, s;
	bool mv;
}info;

vector <pair<int, int>> v;
queue <info> map[50][50];
int N, K;

void print_map() {
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			if (map[r][c].empty()) {
				printf("0 ");
			}
			else
				printf("1 ");
		}
		cout << endl;
	}
	cout << endl;
}

void move_() {
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {

			while (!map[r][c].empty() && !map[r][c].front().mv) {
				info tmp = map[r][c].front();
				tmp.mv = true;	// 이동 완료 상태로 변경
				map[r][c].pop();

				int r_next = (r + N + (dr[tmp.d] * tmp.s) % N) % N;
				int c_next = (c + N + (dc[tmp.d] * tmp.s) % N) % N;

				if (map[r_next][c_next].empty() || !map[r_next][c_next].back().mv) {
					v.push_back(make_pair(r_next, c_next));
				}

				map[r_next][c_next].push(tmp);
			}

		}
	}
}

void split_() {
	while (!v.empty()) {
		int r = v.back().first;
		int c = v.back().second;
		v.pop_back();

		if (map[r][c].size() > 1) {
			int m = 0, s = 0, d = map[r][c].front().d;
			int num = map[r][c].size();

			while(!map[r][c].empty()){
				m += map[r][c].front().m;
				s += map[r][c].front().s;
				if (d >= 0) {
					if ((d + map[r][c].front().d) % 2 == 1) {
						d = -1;
					}
				}
				map[r][c].pop();
			}
			
			if (m / 5 == 0)	continue;

			info tmp;
			tmp.m = m / 5;
			tmp.s = s / num;
			tmp.mv = false;

			if (d == -1) d = 1;
			else d = 0;

			for (int dir = d; dir < 8; dir += 2) {
				tmp.d = dir;
				map[r][c].push(tmp);
			}

		}

		else {
			map[r][c].front().mv = false;
		}
	}
}

void sol() {
	//print_map();
	for (int k = 0; k < K; k++) {
		move_();
		//print_map();
		split_();
	}
}

int main() {
	int M;
	cin >> N >> M >> K;

	for (int i = 0; i < M; i++) {
		int r, c;
		info tmp;
		cin >> r >> c >> tmp.m >> tmp.s >> tmp.d;
		tmp.mv = false;
		map[r - 1][c - 1].push(tmp);
	}

	sol();
	
	int ans = 0;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			while (!map[r][c].empty()) {
				ans += map[r][c].front().m;
				map[r][c].pop();
			}
		}
	}

	cout << ans << endl;
}