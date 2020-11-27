#include <iostream>

using namespace std;

int N, M, K;
pair <int, int > map[20][20];
int dr[4] = { -1,1,0,0 };
int dc[4] = { 0,0,-1,1 };

typedef struct info_ {
	int r, c, d;
	int pd[4][4];
	int life;
}info;

//void print_(info* shark) {
//
//	printf("--------[map]--------\n");
//	for (int r = 0; r < N; r++) {
//		for (int c = 0; c < N; c++) {
//			printf("%3d", map[r][c].first);
//		}
//		cout << endl;
//	}
//	cout << endl;
//
//	printf("--------[map]--------\n");
//	for (int r = 0; r < N; r++) {
//		for (int c = 0; c < N; c++) {
//			printf("%3d", map[r][c].second);
//		}
//		cout << endl;
//	}
//	cout << endl;
//
//	printf("--------[shark]--------\n");
//	for (int m = 0; m < M; m++) {
//		printf("shark(%d) r= %d, c = %d, d = %d\n", shark[m].life, shark[m].r, shark[m].c, shark[m].d);
//		for (int i = 0; i < 4; i++) {
//			for (int j = 0; j < 4; j++) {
//				printf("%d ", shark[m].pd[i][j]);
//			}
//			printf("\n");
//		}
//		printf("\n");
//	}
//}

bool outside(int r, int c) {
	if (r < 0 || c < 0 || r >= N || c >= N)
		return true;
	else
		return false;
}

void decrease_(info* shark) {
	// 현재지도에서 냄새 줄이기
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			if (map[r][c].second == 0) {
				continue;
			}
			else {
				map[r][c].second--;
				if (map[r][c].second == 0)
					map[r][c].first = 0;
			}
		}
	}

	// 상어 현재 위치 냄새 추가
	for (int m = 0; m < M; m++) {
		if (shark[m].life > 0) {
			map[shark[m].r][shark[m].c] = make_pair(shark[m].life, K);
		}
	}
}

void move_(info* shark, int& remain) {
	for (int m = 0; m < M; m++) {
		if (shark[m].life == 0)	continue;

		bool flag = true;

		for (int d_priority = 0; d_priority < 4; d_priority++) {
			int d = shark[m].pd[shark[m].d][d_priority];
			int r = shark[m].r + dr[d];
			int c = shark[m].c + dc[d];
			if (!outside(r, c) && map[r][c].second == 0) {
				if (map[r][c].first != 0) {
					shark[m].life = 0;
					remain--;
					break;
				}
				shark[m].r = r;
				shark[m].c = c;
				shark[m].d = d;
				map[r][c].first = shark[m].life;
				flag = false;
				break;
			}
		}

		if (flag) {
			for (int d_priority = 0; d_priority < 4; d_priority++) {
				int d = shark[m].pd[shark[m].d][d_priority];
				int r = shark[m].r + dr[d];
				int c = shark[m].c + dc[d];
				if (!outside(r, c) && map[r][c].first == shark[m].life) {
					shark[m].r = r;
					shark[m].c = c;
					shark[m].d = d;
					break;
				}
			}
		}
	}
}

int sol(info* shark) {
	int T = 1000, t = 1;
	int remain = M;

	for (t = 1; t <= T; t++) {
		move_(shark, remain);
		if (remain == 1) break;
		// ++++ map_decrease
		decrease_(shark);

	}

	if (t == 1001)	t = -1;
	
	return t;
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M >> K;

	info* shark = new info[M];

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cin >> map[r][c].first;
			if (map[r][c].first != 0) {
				shark[map[r][c].first - 1].life = map[r][c].first;
				shark[map[r][c].first - 1].r = r;
				shark[map[r][c].first - 1].c = c;
			}
		}
	}

	for (int m = 0; m < M; m++) {
		int d;
		cin >> d;
		shark[m].d = d - 1;
	}

	for (int m = 0; m < M; m++) {
		for (int d_cur = 0; d_cur < 4; d_cur++) {
			for (int d_next = 0; d_next < 4; d_next++) {
				int d;
				cin >> d;
				shark[m].pd[d_cur][d_next] = d - 1;
			}
		}
	}

	// ++++ map_decrease
	decrease_(shark);

	//print_(shark);
	int ans = sol(shark);
	//print_(shark);
	cout << ans << endl;

}