
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

typedef struct info {
	int r, c;		// 행 열
	int s, d, z;	// 속력, 이동방향, 크기
	bool alive;
};

int R, C, M;
int ans = 0;

int map[101][101] = { 0 };
int map_copy[101][101] = { 0 };

// up 1, down 2, right 3, left 4
int dx[5] = { 0, -1, 1, 0, 0 };
int dy[5] = { 0, 0, 0, 1, -1 };


void catch_shark(info* shark, int c) {
	for (int i = 1; i <= R; i++) {
		if (map[i][c] != 0) {
			ans += shark[map[i][c]].z;
			shark[map[i][c]].alive = 0;
			//printf("catch %d", map[i][c]);
			break;
		}
	}
}

void eat_shark(info* shark, int idx) {
	
	int r = shark[idx].r;
	int c = shark[idx].c;

	if (map_copy[r][c] != 0) {
		if (shark[(map_copy[r][c])].z > shark[idx].z) {
			shark[idx].alive = 0;
			return;
		}

		else {
			shark[(map_copy[r][c])].alive = 0;
		}
	}

	map_copy[r][c] = idx;

	/*printf("\neat!!\n---------------------\n");
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			printf("%3d", map_copy[i][j]);
		}
		cout << endl;
	}
	printf("\n---------------------\n");*/

	return;
}

//void shark_go(info* shark, int idx, int s_) {
//	int d = shark[idx].d;
//	int s = s_;
//	int c = shark[idx].c;
//	int r = shark[idx].r;
//
//	if (d == 2) {
//		if ((r + s) > 2 * R + 2) {
//			shark[idx].r = r + s - 2 * R + 2;
//		}
//
//		else if ((r + s) > R - 1) {
//			shark[idx].r = 2 * R - r - s;
//			d = 1;
//		}
//
//		else {
//			shark[idx].r = r + s;
//		}
//	}
//
//	else if (d == 1) {
//		if ((r - s) <= -(R - 2)) {
//			//shark[idx].r = R - (r - s - R + 2);
//			shark[idx].r = R - (s - R - r - 2);
//		}
//
//		else if ((r - s) < 2) {
//			shark[idx].r = s - r;
//			d = 2;
//		}
//
//		else {
//			shark[idx].r = r - s;
//		}
//	}
//
//	else if (d == 3) {
//		if ((c + s) > 2 * C + 2) {
//			shark[idx].c = c + s - 2 * C + 2;
//		}
//
//		else if ((c + s) > C - 1) {
//			shark[idx].c = 2 * C - c - s;
//			d = 4;
//		}
//
//		else {
//			shark[idx].c = c + s;
//		}
//	}
//	else if (d == 4) {
//		if ((c - s) <= -(C - 2)) {
//			//shark[idx].c = C - (c - s - C + 2);
//			shark[idx].c = C - (s - C - c - 2);
//		}
//
//		else if ((c - s) < 2) {
//			shark[idx].c = s - c;
//			d = 3;
//		}
//
//		else {
//			shark[idx].c = c - s;
//		}
//	}
//}


//void shark_go(info* shark, int idx, int s_) {
//	
//	int s = s_-1;
//
//	if (shark[idx].d == 1) {
//		shark[idx].r--;
//		if (shark[idx].r == 1)
//			shark[idx].d = 2;
//		if(s > 0)
//			shark_go(shark, idx, s);
//	}
//
//	else if (shark[idx].d == 2) {
//		shark[idx].r++;
//		if (shark[idx].r == R)
//			shark[idx].d = 1;
//		if (s > 0)
//			shark_go(shark, idx, s);
//	}
//
//	else if (shark[idx].d == 4) {
//		shark[idx].c--;
//		if (shark[idx].c == 1)
//			shark[idx].d = 3;
//		if (s > 0)
//			shark_go(shark, idx, s);
//	}
//
//	else if (shark[idx].d == 3) {
//		shark[idx].c++;
//		
//
//		if (shark[idx].c == C) {
//			shark[idx].d = 4;
//			
//		}
//		
//		if (s > 0)
//			shark_go(shark, idx, s);
//	}
//	return;
//}

void shark_go(info* shark, int idx, int s_) {

	int s = s_;
	
	if (s == 0) {
		return;
	}

	int c = shark[idx].c;
	int r = shark[idx].r;
	int d = shark[idx].d;
	
	if (d == 1) {
		
		if (r - s < 2) {
			s = s - r + 1;
			shark[idx].r = 1;
			shark[idx].d = 2;
		}

		else {
			shark[idx].r = r - s;
			s = 0;
		}

		if (s > 0)
			shark_go(shark, idx, s);
	}

	else if (d == 2) {
		if (r + s > R - 1) {
			s = s - (R - r);
			shark[idx].r = R;
			shark[idx].d = 1;
		}

		else {
			shark[idx].r = r + s;
			s = 0;
		}

		if (shark[idx].r == R)
			shark[idx].d = 1;
		if (s > 0)
			shark_go(shark, idx, s);
	}

	else if (d == 4) {
		
		if (c - s < 2) {
			s = s - c + 1;
			shark[idx].c = 1;
			shark[idx].d = 3;
		}
		else {
			shark[idx].c = c - s;
			s = 0;
		}

		if (s > 0)
			shark_go(shark, idx, s);
	}

	else if (d == 3) {
		if (c + s > C - 1) {
			s = s - (C - c);
			shark[idx].c = C;
			shark[idx].d = 4;
		}

		else {
			shark[idx].c = c + s;
			s = 0;
		}

		if (s > 0)
			shark_go(shark, idx, s);
	}
	return;
}


void shark_move(info* shark, int idx) {
	if (shark[idx].alive == 0 ) return;

	else {

		shark_go(shark, idx, shark[idx].s);

		eat_shark(shark, idx);

	}
}

void solve(info* shark) {
	for (int i = 1; i <= C; i++) {

		

		catch_shark(shark, i);

		/*printf("\n---------------------\n");
		for (int i = 1; i <= R; i++) {
			for (int j = 1; j <= C; j++) {
				printf("%3d", map[i][j]);
			}
			cout << endl;
		}
		printf("\n---------------------\n");*/

		for (int j = 1; j <= M; j++)
			shark_move(shark, j);

		for (int k = 1; k <= R; k++) {
			for (int l = 1; l <= C; l++) {
				map[k][l] = map_copy[k][l];
			}
		}


		memset(map_copy, 0, sizeof(map_copy));
	}
}

void input() {
	
	cin >> R >> C;

	cin >> M;

	info* shark = new info[M+1];
	shark[0] = { 0, };

	for (int i = 1; i <= M; i++) {
		cin >> shark[i].r >> shark[i].c >> shark[i].s >> shark[i].d >> shark[i].z;

		shark[i].alive = 1;
		map[shark[i].r][shark[i].c] = i;
		if (shark[i].d == 4 || shark[i].d == 3) {
			shark[i].s = shark[i].s % (2 * C - 2);
		}
		else {
			shark[i].s = shark[i].s % (2 * R - 2);
		}
		
	}

	/*printf("\n---------------------\n");
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			printf("%3d", map[i][j]);
		}
		cout << endl;
	}
	printf("\n---------------------\n");*/

	

	solve(shark);

}

int main() {
	
	input();
	cout << ans << endl;
}