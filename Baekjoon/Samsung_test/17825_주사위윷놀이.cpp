#include <iostream>
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

queue <int> q;

int map[33][6] = {  {0,1,2,3,4,5},			//0
					{2,2,3,4,5,6},			//1
					{4,3,4,5,6,7},			//2
					{6,4,5,6,7,8},			//3
					{8,5,6,7,8,9},			//4
					{10,21,22,23,29,30},	//5
					{12,7,8,9,10,11},			//6
					{14,8,9,10,11,12},
					{16,9,10,11,12,13},
					{18,10,11,12,13,14},
					{20,24,25,29,30,31},	//10
					{22,12,13,14,15,16},
					{24,13,14,15,16,17},
					{26,14,15,16,17,18},
					{28,15,16,17,18,19},
					{30,26,27,28,29,30},	//15
					{32,17,18,19,20,32},
					{34,18,19,20,32,32},
					{36,19,20,32,32,32},
					{38,20,32,32,32,32},
					{40,32,32,32,32,32},	//20
					{13,22,23,29,30,31},	//21 (10에서 go)
					{16,23,29,30,31,20},
					{19,29,30,31,20,32},
					{22,25,29,30,31,20},	//24 (20에서 go)
					{24,29,30,31,20,32},	//25
					{28,27,28,29,30,31},
					{27,28,29,30,31,20},
					{26,29,30,31,20,32},
					{25,30,31,20,32,32},
					{30,31,20,32,32,32},	//30
					{35,20,32,32,32,32},
					{0,32,32,32,32,32}		//32
};

int dice[2][10];
int ans = 0;

int horse[4] = { 0 };

void solve() {
	int tmpans;
	int idx;

	tmpans = 0;
	for (int i = 0; i < 10; i++) {
		idx = dice[1][i];							// 해당하는 말
		
		if (horse[idx] == 32) return;

		horse[idx] = map[horse[idx]][dice[0][i]];	// 말의 위치에 주사위에서 나온값으로 이동
		for (int j = 0; j < 4; j++) {				// 엎어지는거 있으면 return
			if (horse[idx] != 32 && idx != j && horse[idx] == horse[j])
				return;
		}
		tmpans += map[horse[idx]][0];				// 없으면 답에 점수 +
	}
	
	ans = max(ans, tmpans);
}

void combi(int idx) {

	if (idx == 10) {
		memset(horse, 0, sizeof(horse));
		solve();
		return;
	}

	for (int i = 0; i < 4; i++) {
		dice[1][idx] = i;
		combi(idx + 1);
	}

}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);
	
	for (int i = 0; i < 10; i++)
		cin >> dice[0][i];

	combi(0);

	cout << ans << endl;
}