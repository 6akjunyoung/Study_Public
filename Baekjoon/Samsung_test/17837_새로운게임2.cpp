#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define WHITE 0
#define RED 1
#define BLUE 2

typedef struct info {
    int r, c, d;
};

int N, K;
int turn = 0;
int map[12][12] = { 0 };

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

vector <int> v_horse[12][12];
info horse[10];

bool outside(int r, int c) {
    if (r < 0 || c < 0 || r >= N || c >= N)
        return true;
    else
        return false;
}

bool move(int idx) {
    int r_cur = horse[idx].r;
    int c_cur = horse[idx].c;
    int r_next = r_cur + dr[horse[idx].d];
    int c_next = c_cur + dc[horse[idx].d];

    if (outside(r_next, c_next) || map[r_next][c_next] == BLUE) {
        horse[idx].d ^= 0x02;   // dir 뒤집기
        r_next = r_cur + dr[horse[idx].d];
        c_next = c_cur + dc[horse[idx].d];
    }

    if (outside(r_next, c_next) || map[r_next][c_next] == BLUE) {
        return false;
    }

    else if (map[r_next][c_next] == RED) {
        while (true) {
            int tmp = v_horse[r_cur][c_cur].back();
            v_horse[r_cur][c_cur].pop_back();

            v_horse[r_next][c_next].push_back(tmp);
            horse[tmp].r = r_next;
            horse[tmp].c = c_next;

            if (tmp == idx) {
                break;
            }
        }
    }

    else if (map[r_next][c_next] == WHITE) {
        vector <int> v;
        while (true) {
            int tmp = v_horse[r_cur][c_cur].back();
            v_horse[r_cur][c_cur].pop_back();

            v.push_back(tmp);

            horse[tmp].r = r_next;
            horse[tmp].c = c_next;

            if (tmp == idx) {
                break;
            }
        }
        while (!v.empty()) {
            v_horse[r_next][c_next].push_back(v.back());
            v.pop_back();
        }
    }

    if (v_horse[r_next][c_next].size() >= 4)
        return true;
    else
        return false;
}

void sol() {
    //init
    turn = 0;

    while (turn < 1000) {
        turn++;
        for (int i = 0; i < K; i++) {
            if (move(i)) {
                return;
            }
        }
    }
    turn = -1;
    return;
}

int main() {
    cin.tie(NULL);
    cout.tie(NULL);
    ios::sync_with_stdio(false);

    cin >> N >> K;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            cin >> map[r][c];
        }
    }

    for (int i = 0; i < K; i++) {
        int d;
        cin >> horse[i].r >> horse[i].c >> d;
        horse[i].r--;
        horse[i].c--;
        v_horse[horse[i].r][horse[i].c].push_back(i);
        switch (d) {
        case 1:
            horse[i].d = 1;   // R
            break;
        case 2:
            horse[i].d = 3;   // L
            break;
        case 3:
            horse[i].d = 0; // U
            break;
        case 4:
            horse[i].d = 2; // D
            break;
        }
    }

    sol();

    cout << turn << endl;
}