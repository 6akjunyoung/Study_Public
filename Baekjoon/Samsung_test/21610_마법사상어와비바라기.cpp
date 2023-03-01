#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

typedef struct bucket_t
{
	int r, c;
	int water;
	int used;
	struct bucket_t* prev;
	struct bucket_t* next;
}BUCKET;

typedef struct pos_t
{
	int r;
	int c;
}POS;

vector <POS> curCloud;
BUCKET  bucket[51][51];
BUCKET  bucketList;
int     N, M;
int     dr[9] = { 0,0,-1,-1,-1,0,1,1,1 };
int     dc[9] = { 0,-1,-1,0,1,1,1,0,-1 };

void init()
{
	curCloud.push_back({ N - 1, 0 });
	curCloud.push_back({ N - 1, 1 });
	curCloud.push_back({ N - 2, 0 });
	curCloud.push_back({ N - 2, 1 });
	bucket[N - 1][0].used++;
	bucket[N - 1][1].used++;
	bucket[N - 2][0].used++;
	bucket[N - 2][1].used++;
}

bool inside(int r, int c)
{
	return (r >= 0 && c >= 0 && r < N&& c < N);
}

void pushBucketList(int i, int j)
{
	if (bucketList.next)
	{
		bucketList.next->prev = &bucket[i][j];
	}
	bucket[i][j].prev = &bucketList;
	bucket[i][j].next = bucketList.next;
	bucketList.next = &bucket[i][j];
}

void popBucketList(int r, int c)
{
	bucket[r][c].prev->next = bucket[r][c].next;
	if (bucket[r][c].next) bucket[r][c].next->prev = bucket[r][c].prev;
	bucket[r][c].prev = bucket[r][c].next = 0;
}

void updateCloud()
{
	vector <POS> tmpCloud;
	BUCKET* p = bucketList.next;
	while (p)
	{
		if (p->used)
		{
			p->used--;
		}
		else
		{
			p->used++;
			tmpCloud.push_back({ p->r, p->c });
		}
		p = p->next;
	}
	curCloud = tmpCloud;
}

void getSolution(int d, int s)
{
	//���� �̵�
	int curCloudSize = curCloud.size();
	for (int i = 0; i < curCloudSize; i++)
	{
		bucket[curCloud[i].r][curCloud[i].c].used--;
		curCloud[i].r = (curCloud[i].r + (dr[d] * s) % N + N) % N;
		curCloud[i].c = (curCloud[i].c + (dc[d] * s) % N + N) % N;
		bucket[curCloud[i].r][curCloud[i].c].used++;
		// ���� ��ġ�� water++
		bucket[curCloud[i].r][curCloud[i].c].water++;
		if (bucket[curCloud[i].r][curCloud[i].c].water == 2)
		{
			pushBucketList(curCloud[i].r, curCloud[i].c);
		}
	}
	// ���� ��ġ�� water�� �밢���� ���ִ� �ٱ��ϸ�ŭ ���Ǿ� ����
	for (int i = 0; i < curCloudSize; i++)
	{
		for (int dir = 2; dir <= 8; dir += 2)
		{
			int rNext = curCloud[i].r + dr[dir];
			int cNext = curCloud[i].c + dc[dir];
			if (inside(rNext, cNext) && bucket[rNext][cNext].water > 0)
			{
				bucket[curCloud[i].r][curCloud[i].c].water++;
				if (bucket[curCloud[i].r][curCloud[i].c].water == 2)
				{
					pushBucketList(curCloud[i].r, curCloud[i].c);
				}
			}
		}
	}
	// ���� ���� �ƴ� ���� �� 2 �̻��� �������� update
	updateCloud();

	// ���� ��ġ�� water -2
	curCloudSize = curCloud.size();
	for (int i = 0; i < curCloudSize; i++)
	{
		bucket[curCloud[i].r][curCloud[i].c].water -= 2;
		if (bucket[curCloud[i].r][curCloud[i].c].water < 2)
		{
			popBucketList(curCloud[i].r, curCloud[i].c);
		}
	}
}

void inputData()
{
	// N(���� ����), M(��� Ƚ��) �Է�
	scanf("%d %d", &N, &M);

	// �ٱ����� ���� �� �Է�
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int water;
			scanf("%d", &water);
			bucket[i][j] = { i,j,water,0,0,0 };
			if (water >= 2)
			{
				pushBucketList(i, j);
			}
		}
	}
}

int main()
{
	inputData();
	init();
	// �̵������ ���� �Է�
	for (int i = 0; i < M; i++)
	{
		int d, s;
		scanf("%d %d", &d, &s);
		getSolution(d, s);
	}
	int ans = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			ans += bucket[i][j].water;
		}
	}
	printf("%d", ans);
}