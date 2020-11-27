#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int N, M;
int map[50][50];
int visit[50][50];
int numz = 0;	// 0 개수
int time = 0;	// time
int cnt = 0;	// 0 count
int ans = 0x7FFFFFFF;

vector <pair<int, int>> v;	// 바이러스
queue <pair<int, int>> q;	// BFS
vector <pair<int, int>> com;	// 조합

int dr[4] = { 0,0,1,-1 };
int dc[4] = { 1,-1,0,0 };

bool inside(int r, int c) {
	if (r >= 0 && c >= 0 && r < N && c < N)
		return true;

	else
		return false;
}


void BFS(int r, int c) {
	int nr, nc;
	
	for (int i = 0; i < 4; i++) {
		nr = r + dr[i];
		nc = c + dc[i];

		if (inside(nr, nc) && visit[nr][nc] != 1) {

			int data = map[nr][nc];
			if (data != 1) {
				visit[nr][nc] = 1;
				q.push(make_pair(nr, nc));
				if (data == 0) {
					cnt++;
				}
			}

		}
	}
}

void solve() {

	int r, c;
	time = 0;
	cnt = 0;

	while (!q.empty()) {
		q.pop();
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			visit[i][j] = map[i][j];
		}
	}

	for (int i = 0; i < M; i++) {
		r = com[i].first;
		c = com[i].second;

		BFS(r, c);
	}
	time++;

	if (cnt != numz) {

		while (!q.empty()) {
			int qs = q.size();
			int qr, qc;

			for (int i = 0; i < qs; i++) {
				qr = q.front().first;
				qc = q.front().second;
				q.pop();

				BFS(qr, qc);

			}
			time++;

			if (numz == cnt) {
				ans = min(ans, time);
				return;
			}

		}

	}

	if(cnt == numz)
		ans = min(ans, time);

	return;

	// check
	/*for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%3d", visit[i][j]);
		}
		cout << endl;
	}
	cout << endl;*/

}

//int t = 1;
void combi(int idx) {	// M의 조합 찾기

	if (com.size() == M) {

		// combi 확인
		/*printf("#%d\n", t++);
		for (int i = 0; i < com.size(); i++) {
			printf("(%d,%d)", com[i].first, com[i].second);
		}
		cout << endl;*/
		
		solve();
	}

	else {
		for (int i = idx; i < v.size(); i++) {
			com.push_back(make_pair(v[i].first, v[i].second));
			combi(i + 1);
			com.pop_back();
		}
	}

}

void input() {
	
	cin >> N >> M;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			
			if (map[i][j] == 0) {	// zero count
				numz++;
			}

			else if (map[i][j] == 2) {	// v에 바이러스 넣기
				v.push_back(make_pair(i, j));
			}
		}
	}

	if (numz == 0) {
		ans = 0;
		return;
	}

	for (int i = 0; i <= v.size() - M; i++) {
		com.push_back(make_pair(v[i].first, v[i].second));
		combi(i + 1);
		com.pop_back();
	}


}

int main() {

	/*FILE* inputtxt;
	freopen_s(&inputtxt, "input_sample.txt", "r", stdin);*/

	input();

	if (ans == 0x7FFFFFFF)
		ans = -1;

	cout << ans << endl;

}
