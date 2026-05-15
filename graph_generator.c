#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VTX 255 // 최대 정점의 수 
#define INF 9999 // 정점끼리 연결되어있지 않음을 의미하는 무한대 값
#define MAX_WEIGHT 100 // 무작위 가중치의 최대 범위

// 인접 행렬로 그래프 표현
int adj[MAX_VTX][MAX_VTX];



// nVtx 는 생성할 그래프의 정점의 수
// 무작위 가중치 그래프 생성 함수
void randomCompleteGraph(int nVtx) {
	if (nVtx > MAX_VTX) return;

	// 생성할때마다 새로운 그래프가 나오도록 초기화
	static int seeded = 0;
	if (!seeded) {
		srand((unsigned int)time(NULL));
		seeded = 1;
	}

	for (int i = 0; i < nVtx; i++) {
		for (int j = 0; j < nVtx; j++) {
			if (i == j) {
				adj[i][j] = 0; // 자기 자신으로 가는 것은 0
			}
			else if (i < j) {
				// 무작위 가중치 생성(1 ~ MAX_WEIGHT)
				int weight = (rand() % MAX_WEIGHT) + 1;
				adj[i][j] = weight;
				adj[j][i] = weight; // 무방향 그래프이므로 대칭으로 저장
			}
		}
	}
}

// 그래프 출력 함수
void printGraph(int nVtx) {
	printf("- 무작위 가중치 그래프 (정점 수 : %d) -\n", nVtx);
	for (int i = 0; i < nVtx; i++) {
		for (int j = 0; j < nVtx; j++) {
			printf("%3d ", adj[i][j]);
		}
		printf("\n");
	}
}



/* 메인함수 예시
int main() {
	int nVtx = 5; // 정점 5개로 생성

	randomCompleteGraph(nVtx);
	printGraph(nVtx);

	return 0;
}
*/
