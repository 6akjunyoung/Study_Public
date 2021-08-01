#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>

using namespace std;

// 구름 이동
// 구름 위치의 water++
// 구름 위치의 water에 대각선에 물있는 바구니만큼 물의양 증가
// 현재 구름 아닌 구름 중 2 이상인 구름 : 다음구름
// 현재구름 <- 다음구름
// 구름 위치의 water -=2

typedef struct bucket_t
{
	int r, c;
	int water;
	int used;
	int prev;
	int next;
}BUCKET;
BUCKET  bucket[5051];

typedef struct pos_t
{
	int r;
	int c;
}POS;
vector <POS> curCloud;

void init()
{}

void getSolution()
{
}

void inputData()
{
}

int main()
{
}