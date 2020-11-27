#include <iostream>
#include <vector>

#define U 0
#define L 1
#define F 2
#define R 3
#define B 4
#define D 5
#define C 0
#define CC 1

using namespace std;

//           [U]
//             0  1  2
//             7  8  3
//             6  5  4
// [L]		 [F]		[R]		   [B]
//  9 10 11   18 19 20   27 28 29   36 37 38
// 16 17 12   25 26 21   34 35 30   43 44 39
// 15 14 13   24 23 22   33 32 31   42 41 40
//           [D]
//            45 46 47
//            52 53 48
//            51 50 49

vector <pair<int, int>> v;

int T, N;
char cube[54];
const char cube_[54] = {'w','w','w','w','w','w','w','w','w',
					    'g','g','g','g','g','g','g','g','g',
						'r','r','r','r','r','r','r','r','r',
						'b','b','b','b','b','b','b','b','b',
						'o','o','o','o','o','o','o','o','o',
						'y','y','y','y','y','y','y','y','y'};

int spin[6][4][3] = { {{38,37,36},{29,28,27},{20,19,18},{11,10, 9}},	// U
					  {{ 0, 7, 6},{18,25,24},{45,52,51},{40,39,38}},	// L
					  {{ 6, 5, 4},{27,34,33},{47,46,45},{13,12,11}},	// F
					  {{ 4, 3, 2},{36,43,42},{49,48,47},{22,21,20}},	// R
					  {{ 2, 1, 0},{9 ,16,15},{51,50,49},{31,30,29}},	// B
					  {{24,23,22},{33,32,31},{42,41,40},{15,14,13}}};	// D

void sol() {
	int plane, dir;
	int i;

	for(int n = 0; n < N; n++) {
		plane = v[n].first;
		dir = v[n].second;
		
		if (dir == C) {
			// around
			for (int j = 0; j < 3; j++) {
				char temp = cube[spin[plane][3][j]];
				for (i = 3; i > 0; i--) {
					cube[spin[plane][i][j]] = cube[spin[plane][i - 1][j]];
				}
				cube[spin[plane][i][j]] = temp;
			}

			// self
			char tmp[2] = { cube[plane * 9 + 7], cube[plane * 9 + 6] };
			for (i = plane * 9 + 7; i > plane * 9 + 1; i -= 2) {
				cube[i] = cube[i - 2];
				cube[i - 1] = cube[i - 3];
			}
			cube[i] = tmp[0];
			cube[i - 1] = tmp[1];
		}

		else if (dir == CC) {
			// around
			for (int j = 0; j < 3; j++) {
				char temp = cube[spin[plane][0][j]];
				for (i = 0; i < 3; i++) {
					cube[spin[plane][i][j]] = cube[spin[plane][i + 1][j]];
				}
				cube[spin[plane][i][j]] = temp;
			}

			// self
			char tmp[2] = { cube[plane * 9], cube[plane * 9 + 1] };
			for (i = plane * 9; i < plane * 9 + 6; i++) {
				cube[i] = cube[i + 2];
				cube[i + 1] = cube[i + 3];
			}
			cube[i] = tmp[0];
			cube[i + 1] = tmp[1];
		}

		
	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	cin >> T;
	
	while (T--) {
		cin >> N;
		char p_, d_;
		int p, d;

		for (int i = 0; i < N; i++) {
			cin >> p_ >> d_;
			switch (p_) {
			case 'U': p = U;  break;
			case 'L': p = L;  break;
			case 'F': p = F;  break;
			case 'R': p = R;  break;
			case 'B': p = B;  break;
			case 'D': p = D;  break;
			}

			switch (d_) {
			case '+': d = C;  break;
			case '-': d = CC; break;
			}
			v.push_back(make_pair(p, d));
		}

		for (int i = 0; i < 54; i++) {
			cube[i] = cube_[i];
		}

		sol();

		v.clear();

		printf("%c%c%c\n%c%c%c\n%c%c%c\n", cube[0], cube[1], cube[2], cube[7], cube[8], cube[3], cube[6], cube[5], cube[4]);
	}
}