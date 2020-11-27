#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

using namespace std;
int N;

int add(int a[][22], int x, int y, int d1, int d2) {
	
	int i = 0, j = 0;
	int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

	vector <int> arr(5, 0);

	int min_pop=0;
	
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			// 1선거구 
			if ((j <= y) && (i < x + d1) && (i + j < x + y)) {
			//if (i < x + d1 && j <= y && !(i >= x && j >= y - (i - x))) {
				arr[0] += a[i][j];
			}
			
			// 2선거구 
			else if ((j > y) && (i <= x + d2) && (-j + i < -y + x)) {
			//else if (i <= x + d2 && j > y && !(i >= x && j <= y + (i - x))) {
				arr[1] += a[i][j];
			}
			
			// 3선거구 
			else if ((i >= x + d1) && (j < y - d1 + d2) && (-i + j < -x - 2 * d1 + y)) {
			//else if (i >= x + d1 && j < y - d1 + d2 && !(i <= x + d1 + d2 && j >= (y - d1 + d2 - (x + d1 + d2 - i)))) {
				arr[2] += a[i][j];
			}

			//4 선거구
			else if ((i > x + d2) && (j >= y - d1 + d2) && (-i - j < -x - y - 2 * d2)) {
			//else if (i > x + d2 && j >= y - d1 + d2 && !(i <= x + d1 + d2 && j <= (y - d1 + d2 + (x + d1 + d2 - i)))) {

				arr[3] += a[i][j];
			}

			// 5선거구
			else
				arr[4] += a[i][j];
		}
	}

	sort(arr.begin(), arr.end());
	
	return arr[4] -arr[0];
}

int main() {

	int A[22][22] = { 0 };
	int r, c = 0;

	int x = 0, y = 0, d1 = 0, d2 = 0;
	int min_pop = 0x7FFFFFFF, tmp = 0;


	cin >> N;
	for (r = 1; r <= N; r++) {
		for (c = 1; c <= N; c++)
			cin >> A[r][c];
	}

	for (x = 1; x < N - 2; x++) {
		for (y = 2; y < N - 1; y++) {
			for (d1 = 1; (d1 < N - x) || (d1 < y); d1++) {
				for (d2 = 1; (d2 <= N - y) || (d2 <= N - x - d1); d2++) {
					tmp = add(A, x, y, d1, d2);

					if (tmp < min_pop)
						min_pop = tmp;

				}
			}
		}
	}
	
	cout << min_pop << endl;
}