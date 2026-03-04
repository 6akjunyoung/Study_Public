#include <iostream>
#include <string>

using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	if (!(cin >> N)) return 0;

	int* stack = new int[N];
	int stacksize = 0;
	string cmd;

	for (int i = 0; i < N; ++i) {
		cin >> cmd;
		if (cmd == "push") {
			int x; cin >> x;
			stack[stacksize++] = x;
		} else if (cmd == "pop") {
			if (stacksize == 0) cout << -1 << endl;
			else { cout << stack[--stacksize] << endl; }
		} else if (cmd == "size") {
			cout << stacksize << endl;
		} else if (cmd == "empty") {
			cout << (stacksize == 0 ? 1 : 0) << endl;
		} else if (cmd == "top") {
			if (stacksize == 0) cout << -1 << endl;
			else cout << stack[stacksize - 1] << endl;
		}
	}

	delete[] stack;
	return 0;
}
