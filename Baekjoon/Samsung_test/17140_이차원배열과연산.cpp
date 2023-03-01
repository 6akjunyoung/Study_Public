#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

vector <int> v;
vector <pair<int, int>> v2;

int r, c, k;
int ans = 0;
int cnt = 0;
int arr[101][101] = { 0 };
int len[101] = { 0 };
int rlen = 3, clen = 3;
int lentmp = 0;

void rowmake(int i) {

	int l = 0;

	for (int j = 1; j<=100; j++) {

		
		if (!v2.empty()) {
			arr[i][j++] = v2.back().first;
			l++;

			if (i == 100)
				break;

			arr[i][j] = v2.back().second;
			v2.pop_back();
			l++;
		}

		else {
			arr[i][j] = 0;
			
		}
		
	}
	while(!v2.empty()) {
		v2.pop_back();
	}

	lentmp = max(l, lentmp);

}

void colmake(int j) {

	int l = 0;

	for (int i = 1; i<=100 ; i++) {
		if (!v2.empty()) {
			arr[i++][j] = v2.back().first;
			l++;

			if (i == 100)
				break;

			arr[i][j] = v2.back().second;
			v2.pop_back();
			l++;
		}

		else {
			arr[i][j] = 0;
			
		}
	}

	while (!v2.empty()) {
		v2.pop_back();
	}

	lentmp = max(l, lentmp);

}

bool comp(pair<int,int> a, pair<int, int> b){
	if (a.second > b.second) {
		return true;
	}

	else if (a.second == b.second && a.first > b.first)
		return true;

	else
		return false;
}

void v2make() {
	int num;
	int s = v.size();

	for(int i = 0; i<s; i++){
		num = v.back();
		v2.push_back(make_pair(num, len[num]));
		v.pop_back();
	}

	while (!v.empty()) {
		v.pop_back();
	}

	sort(v2.begin(), v2.end(), comp);
	
	
}

void row(int i) {
	int flag = 1;

	for (int j = 1; j <= clen; j++) {

		if (arr[i][j] != 0) {
			flag = 1;

			len[arr[i][j]]++;
			if (v.empty()) {
				v.push_back(arr[i][j]);
			}

			else {
				for (int k = 0; k < v.size(); k++) {
					if (v[k] == arr[i][j]) {
						flag = 0;
						break;
					}
				}

				if (flag == 1)
					v.push_back(arr[i][j]);
			}
		}

	}
}

void col(int j) {
	int flag = 1;

	for (int i = 1; i <= rlen; i++) {

		if (arr[i][j] != 0) {
			flag = 1;


			len[arr[i][j]]++;
			if (v.empty()) {
				v.push_back(arr[i][j]);
			}

			else {
				for (int k = 0; k < v.size(); k++) {
					if (v[k] == arr[i][j]) {
						
						flag = 0;
						break;
					}
				}
				
				if (flag == 1) {
					v.push_back(arr[i][j]);

				}
			}
		}
	}
}


void solve() {
	while (cnt < 101) {

		lentmp = 0;

		for (int i = 1; i <= max(rlen, clen); i++) {
						
			memset(len, 0, sizeof(len));

			// 행 >= 열
			if (rlen >= clen){
				row(i);

				v2make();
				
				rowmake(i);

			}

			// 행 < 열
			else{
				col(i);

				v2make();

				colmake(i);

			}

		}


		if (rlen >= clen)
			clen = lentmp;
		else
			rlen = lentmp;


		
		

		cnt++;
		
		// 출력
		//cout << endl;
		//printf("cnt : %3d, r: %3d, c: %3d", cnt, rlen, clen);
		//cout << endl;
		//for (int i = 1; i <= rlen; i++) {
		//	for (int j = 1; j <= clen; j++) {
		//		printf("%3d", arr[i][j]);
		//	}
		//	cout << endl;
		//}

		if(arr[r][c] == k){
			ans = cnt;
			return;
		}

	}
	ans = -1;
	return;
}

void input() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> r >> c >> k;
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			cin >> arr[i][j];
		}
	}
}

int main() {
	input();

	if(arr[r][c] != k)
		solve();

	cout << ans << endl;

}
