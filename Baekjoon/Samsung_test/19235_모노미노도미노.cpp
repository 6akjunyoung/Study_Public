#include <iostream>
#include <vector>

#define GREEN 0
#define BLUE 1

using namespace std;

int map[2][6][4] = { 0 };

typedef struct info_ {
	int t, r, c;
}info;

vector <info> block_list;
int ans = 0;
int N;

void print_map() {
	printf("---------GREEN---------\n");
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 4; c++) {
			if (map[GREEN][r][c] == 0)
				printf("%3c", '*');
			else
				printf("%3d", map[GREEN][r][c]);
		}
		cout << endl;
	}
	cout << endl;

	printf("---------BLUE---------\n");
	for (int r = 0; r < 6; r++) {
		for (int c = 0; c < 4; c++) {
			if(map[BLUE][r][c] == 0)
				printf("%3c", '*');
			else
				printf("%3d", map[BLUE][r][c]);
		}
		cout << endl;
	}
	cout << endl;
}

bool outside(int r, int c) {
	if (r < 0 || c < 0 || r >= 6 || c >= 4)
		return true;
	else
		return false;
}

void shift_block(int color) {
	int erase_cnt = 0;
	for (int r = 0; r < 2; r++) {
		for (int c = 0; c < 4; c++) {
			if (map[color][r][c] != 0) {
				erase_cnt++;
				break;
			}
		}
	}
	for (int cnt = 0; cnt < erase_cnt; cnt++) {
		for (int r = 5; r >= 0; r--) {
			for (int c = 0; c < 4; c++) {
				if (r == 0)
					map[color][r][c] = 0;
				else
					map[color][r][c] = map[color][r - 1][c];
			}
		}
	}

	return;
}

void block_down(int color) {
	bool flag = true;

	while (flag) {
		flag = false;
		for (int r = 4; r >= 0; r--) {
			for (int c = 0; c < 4; c++) {

				if (map[color][r][c] != 0) { // block exist

					if (!outside(r, c + 1) && map[color][r][c] == map[color][r][c + 1]) {			// 2
						if (map[color][r + 1][c] == 0 && map[color][r + 1][c + 1] == 0) {
							map[color][r + 1][c] = map[color][r + 1][c + 1] = map[color][r][c];
							map[color][r][c] = map[color][r][c + 1] = 0;
							flag = true;
						}
					}
					else if (!outside(r, c - 1) && map[color][r][c] == map[color][r][c - 1]) continue;

					else if (map[color][r + 1][c] == 0) {
						map[color][r + 1][c] = map[color][r][c];									// 1
						map[color][r][c] = 0;
						flag = true;
						if (!outside(r - 1, c) && map[color][r - 1][c] == map[color][r + 1][c]) {	// 3
							map[color][r][c] = map[color][r - 1][c];
							map[color][r - 1][c] = 0;
						}
					}
				}


			}
		}
	}
}

bool score_check(int color) {
	bool flag = true;
	int ans_ = 0;

	// 다 차있는 행 지우기
	for (int r = 0; r < 6; r++) {
		flag = true;

		for (int c = 0; c < 4; c++) {
			if (map[color][r][c] == 0) {
				flag = false;
				break;
			}
		}

		if (flag) {
			for (int r_ = r; r_ >= 0; r_--) {
				for (int c_ = 0; c_ < 4; c_++) {
					if (r_ == 0)
						map[color][r_][c_] = 0;
					else
						map[color][r_][c_] = map[color][r_ - 1][c_];
				}
			}
			ans_++;
		}
	}

	if (ans_ > 0) {
		ans += ans_;
		return true;
	}

	else
		return false;
}

void put_block(info block, int color, int n) {

	// init
	int block_shape = block.t;
	int block_c = block.c;

	if (color == BLUE) {
		block_c = block.r;
		switch (block_shape) {
		case 2: block_shape = 3; break;
		case 3: block_shape = 2; break;
		default: break;
		}
	}

	// put block
	switch (block_shape) {

	case 1:
		for (int r = 0; r < 6; r++) {
			if (r == 5) {
				map[color][r][block_c] = n;
			}

			else if (map[color][r + 1][block_c] != 0) {
				map[color][r][block_c] = n;
				break;
			}
		}
		break;

	case 2:
		for (int r = 0; r < 6; r++) {
			if (r == 5) {
				map[color][r][block_c] = map[color][r][block_c + 1] = n;
			}

			else if (map[color][r + 1][block_c] != 0 || map[color][r + 1][block_c + 1] != 0) {
				map[color][r][block_c] = map[color][r][block_c + 1] = n;
				break;
			}
		}
		break;

	case 3:
		for (int r = 0; r < 5; r++) {
			if (r == 4) {
				map[color][r][block_c] = map[color][r + 1][block_c] = n;
			}

			else if (map[color][r + 2][block_c] != 0) {
				map[color][r][block_c] = map[color][r + 1][block_c] = n;
				break;
			}
		}
		break;

	default: break;
	}
}

void sol() {
	for (int n = 0; n < N; n++) {

		info block = block_list[n];
		put_block(block, GREEN, n + 1);
		put_block(block, BLUE, n + 1);

		//
		//printf("[%d] shape:%d, r:%d, c:%d\n", n + 1, block_list[n].t, block_list[n].r, block_list[n].c);
		//print_map();

		while (score_check(GREEN))
			block_down(GREEN);

		while (score_check(BLUE))
			block_down(BLUE);

		//
		//print_map();

		shift_block(GREEN);
		shift_block(BLUE);
		//
		//print_map();

	}
}

int main() {
	cin.tie(NULL);
	cout.tie(NULL);
	ios::sync_with_stdio(false);

	FILE* input_;
	//freopen_s(&input_, "input_sample.txt", "rt", stdin);

	info block;

	cin >> N;
	for (int n = 0; n < N; n++) {
		cin >> block.t >> block.r >> block.c;
		block_list.push_back(block);
	}
	sol();

	int remain = 0;
	for (int color = 0; color < 2; color++) {
		for (int r = 0; r < 6; r++) {
			for (int c = 0; c < 4; c++) {
				if (map[color][r][c]) remain++;
			}
		}
	}

	cout << ans << endl << remain << endl;
}