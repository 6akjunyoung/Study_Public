#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

typedef struct _info {
	int r, c;
	int size, size_cnt;
}info; 

vector <pair<int, int>> v;
queue <pair<int, int>> q;

int N;
int map[20][20] = { 0 };
int visit[20][20] = { -1 };
int ans = 0;
int dr[4] = { 0,1,0,-1 };
int dc[4] = { 1,0,-1,0 };

info shark = { 0,0,2,0 };

bool outside(int r, int c) {
	if (r < 0 || c < 0 || r >= N || c >= N)
		return true;
	else
		return false;
}

bool comp(pair<int, int> x, pair<int, int> y) {
	if (x.first < y.first)
		return true;

	else if (x.first == y.first && x.second < y.second)
		return true;

	else
		return false;
}

bool can_eat() {
	// if can eat v one
	if (v.empty())
		return false;

	else {
		sort(v.begin(), v.end(), comp);
		while (v.size() != 1) {
			v.pop_back();
		}

		return true;
	}
}

void eat() {

	shark.r = v.back().first;
	shark.c = v.back().second;
	map[shark.r][shark.c] = 0;
	shark.size_cnt++;

	if (shark.size == shark.size_cnt) {
		shark.size++;
		shark.size_cnt = 0;
	}

	ans += visit[shark.r][shark.c];

	//init
	memset(visit, -1, sizeof(visit));

	while (!v.empty())
		v.pop_back();

	while (!q.empty())
		q.pop();

	q.push(make_pair(shark.r, shark.c));
	visit[shark.r][shark.c] = 0;
}

void BFS() {
	int r = q.front().first;
	int c = q.front().second;
	q.pop();

	for (int idx = 0; idx < 4; idx++) {
		int r_ = r + dr[idx];
		int c_ = c + dc[idx];

		if (!outside(r_, c_) && visit[r_][c_] == -1) {
			if (map[r_][c_] <= shark.size) {
				if(map[r_][c_] == 0 || map[r_][c_] == shark.size)
					q.push(make_pair(r_, c_));

				else if (map[r_][c_] < shark.size)
					v.push_back(make_pair(r_, c_));	

				visit[r_][c_] = visit[r][c] + 1;
			}
		}
	}
}

void sol() {
	q.push(make_pair(shark.r, shark.c));
	memset(visit, -1, sizeof(visit));

	visit[shark.r][shark.c] = 0;
	while (true) {
		int qsize = q.size();
		while (qsize--) {
			BFS();
		}
		
		if (can_eat()) {
			eat();
		}

		else if (q.empty()) {
			break;
		}
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N;

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cin >> map[r][c];
			if (map[r][c] == 9) {
				map[r][c] = 0;
				shark.r = r;
				shark.c = c;
			}
		}
	}
	sol();

	cout << ans << endl;
}