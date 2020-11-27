#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int N, M;
vector <pair<int, int>> home;
vector <pair<int, int>> chicken;
bool check[13] = { false };
int ans = 0x7FFFFFFF;

void sol() {
	int home_n = home.size();
	int chicken_n = chicken.size();
	int min_dist = 100;
	int distance = 0;

	for (int i = 0; i < home_n; i++) {
		min_dist = 100;	// max distance
		for (int idx = 0; idx < chicken_n; idx++) {
			
			if (check[idx] == false)
				continue;

			int tmp_dist = abs(home[i].first - chicken[idx].first) + abs(home[i].second - chicken[idx].second);
			min_dist = min_dist < tmp_dist ? min_dist : tmp_dist;
		}
		distance += min_dist;
	}
	ans = ans < distance ? ans : distance;
}

void combi(int idx, int m) {
	if (idx == chicken.size()) {
		if (m == M)
			sol();

		return;
	}
	check[idx] = true;
	combi(idx + 1, m + 1);

	check[idx] = false;
	combi(idx + 1, m);
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> N >> M;

	int tmp;

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cin >> tmp;
			if (tmp == 1)
				home.push_back(make_pair(r, c));

			else if (tmp == 2)
				chicken.push_back(make_pair(r, c));
		}
	}

	combi(0, 0);

	cout << ans << endl;
}