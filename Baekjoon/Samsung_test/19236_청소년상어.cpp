#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct info_ {
    int r, c, d;
    int num, cnt;
}info;

typedef struct state_ {
    info_ shark;
    pair<int, int> map[4][4];
}state;

int dr[8] = { -1,-1,0,1,1,1,0,-1 };
int dc[8] = { 0,-1,-1,-1,0,1,1,1 };
int ans = 0;

//void print_(const state state_) {
//    printf("\n-----------------------\n");
//    cout << "[shark]\n"
//        << "r: " << state_.shark.r
//        << " | c: " << state_.shark.c
//        << " | d: " << state_.shark.d
//        << " | cnt: " << state_.shark.cnt
//        << " | num: " << state_.shark.num << endl << endl;
//    cout << "[map]\n";
//    for (int r = 0; r < 4; r++) {
//        for (int c = 0; c < 4; c++) {
//            printf("%3d", state_.map[r][c].first);
//        }
//        cout << endl;
//    }
//    cout << endl;
//
//    printf("\n-----------------------\n");
//}

bool outside(int r, int c) {
    if (r < 0 || c < 0 || r >= 4 || c >= 4)
        return true;
    else
        return false;
}

state eat_(const state state_, int r, int c) {
    state state;

    // map copy
    for (int r_ = 0; r_ < 4; r_++) {
        for (int c_ = 0; c_ < 4; c_++) {
            state.map[r_][c_] = state_.map[r_][c_];   //
        }
    }

    // shark
    state.shark.num = state_.shark.num + (state_.map[r][c].first + 1);
    state.shark.d = state_.map[r][c].second;
    state.shark.r = r;
    state.shark.c = c;
    state.shark.cnt = state_.shark.cnt + 1;

    // eat
    state.map[r][c].first = -state.shark.cnt;

    return state;
}

void sol(state past, int r, int c) {

    //eat
    state cur = eat_(past, r, c);

    bool flag = false;
    int fish_number = 0;
    int dir_cnt = 8;

    // fish move()
    while (fish_number < 16) {
        flag = false;

        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                // 해당 idx일 때
                if (cur.map[r][c].first == fish_number) {

                    // 8방향
                    for (dir_cnt = 0; dir_cnt < 8; dir_cnt++) {

                        int d_next = (cur.map[r][c].second + dir_cnt) % 8;
                        int r_next = r + dr[d_next];
                        int c_next = c + dc[d_next];

                        if (outside(r_next, c_next))   continue;
                        else if (r_next == cur.shark.r && c_next == cur.shark.c) continue;
                        else {
                            cur.map[r][c].second = d_next;
                            swap(cur.map[r][c], cur.map[r_next][c_next]);
                            break;
                        }
                    }
                    flag = true;
                    break;
                }

            }
            if (flag)   break;
        }
        fish_number++;
    }

    // if shark can move, sol()
    flag = true;
    for (int i = 1; i < 4; i++) {
        int r_next = cur.shark.r + dr[cur.shark.d] * i;
        int c_next = cur.shark.c + dc[cur.shark.d] * i;
        // can eat
        if (outside(r_next, c_next))   break;
        else if (cur.map[r_next][c_next].first >= 0) {
            flag = false;
            sol(cur, r_next, c_next);
        }

    }

    //다 돌아도 없으면 ans
    if (flag)
        ans = max(ans, cur.shark.num);
}

int main() {
    state cur;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            cin >> cur.map[r][c].first >> cur.map[r][c].second;
            cur.map[r][c].first--;
            cur.map[r][c].second--;
        }
    }

    cur.shark = { 0,0,0,0,0 };

    sol(cur, 0, 0);

    cout << ans << endl;
}