#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>

using namespace std;

// ���� �̵�
// ���� ��ġ�� water++
// ���� ��ġ�� water�� �밢���� ���ִ� �ٱ��ϸ�ŭ ���Ǿ� ����
// ���� ���� �ƴ� ���� �� 2 �̻��� ���� : ��������
// ���籸�� <- ��������
// ���� ��ġ�� water -=2

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