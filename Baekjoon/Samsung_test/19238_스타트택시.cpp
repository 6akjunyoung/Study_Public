#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

#define SRC 1
#define DST 2

using namespace std;

typedef struct info_ {
	int r, c, num, fuel;
}info;

typedef struct destination_ {
	int r, c;
}destination;

int N, M;

int dr[4] = { 0,0,-1,1 };
int dc[4] = { 1,-1,0,0 };

int map_src[20][20] = { 0 };
int map_dst[20][20] = { 0 };

info taxi = { 0 };

void print_(int map[][20]) {
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			printf("%3d", map[r][c]);
		}
		cout << endl;
	}
	cout << endl;
}

bool comp(pair<int, int> x, pair<int, int>y) {
	if (x.first < y.first)
		return true;
	else if (x.first == y.first && x.second < y.second)
		return true;
	else 
		return false;
}

bool outside(int r, int c) {
	if (r < 0 || c < 0 || r >= N || c >= N)
		return true;
	else
		return false;
}

void BFS(destination* dst) {
	vector <pair <int, int>> v;
	queue <pair <int, int>> q;

	int r = taxi.r;
	int c = taxi.c;
	int fuel = 0;
	int map[20][20] = { 0 };
	
	// src
	if (taxi.num == -1) {
		if (map_src[r][c] > 0) {
			taxi.num = map_src[r][c];
			map_src[r][c] = 0;
			return;
		}
			
		else {
			// map copy
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					map[i][j] = map_src[i][j];
				}
			}

			q.push(make_pair(taxi.r, taxi.c));
			map[r][c] = -1;

			while (!q.empty()) {
				//print_(map);
				int qs = q.size();
				fuel++;
				for (int i = 0; i < qs; i++) {
					r = q.front().first;
					c = q.front().second;
					q.pop();
					for (int d = 0; d < 4; d++) {
						int r_ = r + dr[d];
						int c_ = c + dc[d];
						if (!outside(r_, c_)) {
							if (map[r_][c_] > 0) {
								v.push_back(make_pair(r_, c_));
							}

							else if (map[r_][c_] != -1) {
								map[r_][c_] = -1;
								q.push(make_pair(r_, c_));
							}
						}
					}
				}

				if (fuel > taxi.fuel) {
					taxi.fuel = -1;
					return;
				}

				if (!v.empty()) {
					sort(v.begin(), v.end(), comp);
					taxi.r = v[0].first;
					taxi.c = v[0].second;
					taxi.fuel -= fuel;
					taxi.num = map[taxi.r][taxi.c];
					map_src[taxi.r][taxi.c] = 0;
					return;
				}				
			}
		}
	}

	// dst
	else {
		if (dst[taxi.num].r == r && dst[taxi.num].c == c) {
			return;
		}

		else {
			// map copy
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (map_src[i][j] == -1) {
						map[i][j] = -1;
					}
					else if (dst[taxi.num].r == i && dst[taxi.num].c == j)
						map[i][j] = taxi.num;

					else if (map_src[i][j] == 0)
						map[i][j] = 0;
				}
			}

			q.push(make_pair(taxi.r, taxi.c));
			map[r][c] = -1;

			while (!q.empty()) {
				//print_(map);
				int qs = q.size();
				fuel++;
				for (int i = 0; i < qs; i++) {
					r = q.front().first;
					c = q.front().second;
					q.pop();
					for (int d = 0; d < 4; d++) {
						int r_ = r + dr[d];
						int c_ = c + dc[d];
						if (!outside(r_, c_)) {
							if (map[r_][c_] == taxi.num) {
								if (fuel > taxi.fuel) {
									taxi.fuel = -1;
									return;
								}
								else {
									taxi.fuel += fuel;
									taxi.r = r_;
									taxi.c = c_;
									return;
								}
							}

							else if (map[r_][c_] != -1) {
								map[r_][c_] = -1;
								q.push(make_pair(r_, c_));
							}
						}
					}
				}
			}
		}
	}

	taxi.fuel = -1;
	return;
}

void sol(destination* dst) {
	int cnt = 0;
	
	/*printf("---------src---------\n");
	print_(map_src);
	printf("---------dst---------\n");
	print_(map_dst);*/

	while (cnt < M) {
		taxi.num = -1;
		
		BFS(dst);
		if (taxi.fuel < 0) break;
		/*printf("---------src---------\n");
		print_(map_src);
		printf("---------dst---------\n");
		print_(map_dst);*/

		BFS(dst);
		if (taxi.fuel < 0) break;
		/*printf("---------src---------\n");
		print_(map_src);
		printf("---------dst---------\n");
		print_(map_dst);*/

		cnt++;
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M >> taxi.fuel;

	destination* dst = new destination[M + 1];

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			int tmp;
			cin >> tmp;
			if (tmp == 1) {
				map_src[r][c] = -1;
			}
		}
	}

	cin >> taxi.r >> taxi.c;
	taxi.r--;
	taxi.c--;

	for (int m = 1; m <= M; m++) {
		int r, c;
		cin >> r >> c;
		map_src[r - 1][c - 1] = m;
		cin >> r >> c;
		dst[m].r = r - 1;
		dst[m].c = c - 1;
	}

	sol(dst);

	cout << taxi.fuel << endl;
	free(dst);
}