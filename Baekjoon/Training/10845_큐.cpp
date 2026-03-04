#include <iostream>
#include <string>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;

    int* queue = new int[N];
    int head = 0;
    int tail = 0;
    int qsize = 0;

    string cmd;
    for (int i = 0; i < N; ++i) {
        cin >> cmd;
        if (cmd == "push") {
            int x; cin >> x;
            queue[tail] = x;
            tail = (tail + 1) % N;
            ++qsize;
        } else if (cmd == "pop") {
            if (qsize == 0) cout << -1 << '\n';
            else {
                cout << queue[head] << '\n';
                head = (head + 1) % N;
                --qsize;
            }
        } else if (cmd == "size") {
            cout << qsize << '\n';
        } else if (cmd == "empty") {
            cout << (qsize == 0 ? 1 : 0) << '\n';
        } else if (cmd == "front") {
            if (qsize == 0) cout << -1 << '\n';
            else cout << queue[head] << '\n';
        } else if (cmd == "back") {
            if (qsize == 0) cout << -1 << '\n';
            else {
                int idx = (tail - 1 + N) % N;
                cout << queue[idx] << '\n';
            }
        }
    }

    delete[] queue;
    return 0;
}
