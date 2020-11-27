#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cstdio>

using namespace std;

int N;
int map[101][101] = { 0 };

int dx[4] = { 1,0,-1,0 };
int dy[4] = { 0,-1,0,1 };

typedef struct info {
	int x, y;
	vector <int> v;
	int g;
}info;

void printmap() {
	cout << endl;
	for (int i = 0; i <= 20; i++) {
		for (int j = 0; j <= 20; j++) {
			printf("%3d", map[i][j]);
		}
		cout << endl;
	}
}

void curve(info* dragon, int idx, int gen) {
	int x, y, d;
	

	if (gen == 0) {

		// 해당위치 색칠
		x = dragon[idx].x;
		y = dragon[idx].y;
		map[y][x]++;

		// 방향뽑기
		d = dragon[idx].v[0];
	
		// x,y 이동
		dragon[idx].x = x = dragon[idx].x + dx[d];
		dragon[idx].y = y = dragon[idx].y + dy[d];
		
		// 방향 추가
		//dragon[idx].v.push_back(d);

		// 색칠
		map[y][x]++;

		//printmap();
	}

	else {
		// x,y
		
		int vs = dragon[idx].v.size();

		for (int i = vs - 1; i >= 0; i--) {

			// 마지막부터 방향 전환
			d = (dragon[idx].v[i] + 1) % 4;
			dragon[idx].v.push_back(d);

			// 이동
			dragon[idx].x = x = dragon[idx].x + dx[d];
			dragon[idx].y = y = dragon[idx].y + dy[d];

			// +1
			map[y][x]++;

			//printmap();
		}
	}
	return;
}

void solve(info* dragon) {
	int ans = 0;

	for (int i = 0; i < N; i++) {
		int g = dragon[i].g;
		for (int j = 0; j <= g; j++) {
			curve(dragon, i, j);
		}
	}

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			if (map[i][j] != 0 && map[i][j + 1] != 0 && map[i + 1][j] != 0 && map[i + 1][j + 1] != 0)
				ans++;
		}
	}

	cout << ans << endl;

}

void input() {
	cin >> N;
	info* dragon = new info[N];

	for (int i = 0; i < N; i++) {
		int d;
		cin >> dragon[i].x >> dragon[i].y >> d >> dragon[i].g;
		dragon[i].v.push_back(d);
	}

	solve(dragon);

	
}

int main() {
	input();

	return 0;
}