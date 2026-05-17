#include <stdio.h>
#include <stdlib.h>

#define V 7
#define MAX_EDGES 50
#define INF 999999

enum {
    GATE,
    SUNGGYEOL,
    STUDENT,
    JUNGSAENG,
    MEMORIAL,
    JAERIM,
    LIBRARY
};

const char *names[V] = {
    "정문",
    "성결관",
    "학생회관",
    "중생관",
    "기념관",
    "재림관",
    "학술정보관"
};

typedef struct {
    int u;
    int v;
    int weight;
} Edge;

int graph[V][V];
Edge edges[MAX_EDGES];
int edgeCount = 0;

void initGraph(void)
{
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = INF;
        }
        graph[i][i] = 0;
    }
}

void addEdge(int u, int v, int weight)
{
    graph[u][v] = weight;
    graph[v][u] = weight;

    edges[edgeCount].u = u;
    edges[edgeCount].v = v;
    edges[edgeCount].weight = weight;
    edgeCount++;
}

void makeCampusGraph(void)
{
    initGraph();

    addEdge(GATE, SUNGGYEOL, 3);
    addEdge(GATE, MEMORIAL, 7);
    addEdge(GATE, JAERIM, 5);

    addEdge(SUNGGYEOL, STUDENT, 1);
    addEdge(SUNGGYEOL, JUNGSAENG, 2);
    addEdge(SUNGGYEOL, MEMORIAL, 5);
    addEdge(SUNGGYEOL, JAERIM, 4);

    addEdge(STUDENT, JUNGSAENG, 2);

    addEdge(JUNGSAENG, MEMORIAL, 3);

    addEdge(MEMORIAL, JAERIM, 3);

    addEdge(JAERIM, LIBRARY, 1);
}

void printVertexMenu(void)
{
    printf("\n[건물 번호]\n");
    for (int i = 0; i < V; i++) {
        printf("%d. %s\n", i, names[i]);
    }
}

int findParent(int parent[], int x)
{
    if (parent[x] == x) {
        return x;
    }

    parent[x] = findParent(parent, parent[x]);
    return parent[x];
}

void unionSet(int parent[], int a, int b)
{
    int rootA = findParent(parent, a);
    int rootB = findParent(parent, b);

    if (rootA != rootB) {
        parent[rootB] = rootA;
    }
}

int compareEdge(const void *a, const void *b)
{
    Edge *e1 = (Edge *)a;
    Edge *e2 = (Edge *)b;

    return e1->weight - e2->weight;
}

void kruskalMST(void)
{
    int parent[V];
    int totalCost = 0;
    int selected = 0;
    Edge sortedEdges[MAX_EDGES];

    for (int i = 0; i < edgeCount; i++) {
        sortedEdges[i] = edges[i];
    }

    qsort(sortedEdges, edgeCount, sizeof(Edge), compareEdge);

    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    printf("\n[Kruskal MST]\n");

    for (int i = 0; i < edgeCount; i++) {
        int u = sortedEdges[i].u;
        int v = sortedEdges[i].v;
        int w = sortedEdges[i].weight;

        if (findParent(parent, u) != findParent(parent, v)) {
            unionSet(parent, u, v);

            printf("%s -- %s : %d분\n", names[u], names[v], w);

            totalCost += w;
            selected++;
        }

        if (selected == V - 1) {
            break;
        }
    }

    printf("총 비용: %d분\n", totalCost);
}

void primMST(void)
{
    int selected[V] = {0};
    int totalCost = 0;
    int edgeUsed = 0;

    selected[0] = 1;

    printf("\n[Prim MST]\n");

    while (edgeUsed < V - 1) {
        int min = INF;
        int u = -1;
        int v = -1;

        for (int i = 0; i < V; i++) {
            if (selected[i]) {
                for (int j = 0; j < V; j++) {
                    if (!selected[j] && graph[i][j] < min) {
                        min = graph[i][j];
                        u = i;
                        v = j;
                    }
                }
            }
        }

        if (u != -1 && v != -1) {
            printf("%s -- %s : %d분\n", names[u], names[v], min);

            selected[v] = 1;
            totalCost += min;
            edgeUsed++;
        }
    }

    printf("총 비용: %d분\n", totalCost);
}

void boruvkaMST(void)
{
    int parent[V];
    int totalCost = 0;
    int numTrees = V;

    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    printf("\n[Boruvka MST]\n");

    while (numTrees > 1) {
        int cheapest[V];

        for (int i = 0; i < V; i++) {
            cheapest[i] = -1;
        }

        for (int i = 0; i < edgeCount; i++) {
            int u = edges[i].u;
            int v = edges[i].v;
            int w = edges[i].weight;

            int setU = findParent(parent, u);
            int setV = findParent(parent, v);

            if (setU == setV) {
                continue;
            }

            if (cheapest[setU] == -1 || edges[cheapest[setU]].weight > w) {
                cheapest[setU] = i;
            }

            if (cheapest[setV] == -1 || edges[cheapest[setV]].weight > w) {
                cheapest[setV] = i;
            }
        }

        for (int i = 0; i < V; i++) {
            if (cheapest[i] != -1) {
                int edgeIndex = cheapest[i];

                int u = edges[edgeIndex].u;
                int v = edges[edgeIndex].v;
                int w = edges[edgeIndex].weight;

                int setU = findParent(parent, u);
                int setV = findParent(parent, v);

                if (setU != setV) {
                    unionSet(parent, setU, setV);

                    printf("%s -- %s : %d분\n", names[u], names[v], w);

                    totalCost += w;
                    numTrees--;
                }
            }
        }
    }

    printf("총 비용: %d분\n", totalCost);
}

void printPath(int parent[], int start, int end)
{
    if (end == -1) {
        return;
    }

    if (end == start) {
        printf("%s", names[start]);
        return;
    }

    printPath(parent, start, parent[end]);
    printf(" -> %s", names[end]);
}

void dijkstra(int start, int end)
{
    int distance[V];
    int visited[V] = {0};
    int parent[V];

    for (int i = 0; i < V; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    distance[start] = 0;

    for (int count = 0; count < V - 1; count++) {
        int min = INF;
        int current = -1;

        for (int i = 0; i < V; i++) {
            if (!visited[i] && distance[i] < min) {
                min = distance[i];
                current = i;
            }
        }

        if (current == -1) {
            break;
        }

        visited[current] = 1;

        for (int next = 0; next < V; next++) {
            if (!visited[next] && graph[current][next] != INF) {
                if (distance[current] + graph[current][next] < distance[next]) {
                    distance[next] = distance[current] + graph[current][next];
                    parent[next] = current;
                }
            }
        }
    }

    printf("\n[Dijkstra]\n");
    printf("최단거리: %d분\n", distance[end]);
    printf("최단경로: ");
    printPath(parent, start, end);
    printf("\n");
}

void floydWarshall(int start, int end)
{
    int dist[V][V];
    int next[V][V];

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];

            if (graph[i][j] != INF && i != j) {
                next[i][j] = j;
            } else {
                next[i][j] = -1;
            }
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }
    }

    printf("\n[Floyd-Warshall]\n");
    printf("최단거리: %d분\n", dist[start][end]);

    printf("최단경로: ");
    if (next[start][end] == -1) {
        printf("경로 없음\n");
        return;
    }

    int current = start;
    printf("%s", names[current]);

    while (current != end) {
        current = next[current][end];
        printf(" -> %s", names[current]);
    }

    printf("\n");
}

void bellmanFord(int start, int end)
{
    int distance[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    distance[start] = 0;

    for (int i = 0; i < V - 1; i++) {
        for (int j = 0; j < edgeCount; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].weight;

            if (distance[u] != INF && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                parent[v] = u;
            }

            if (distance[v] != INF && distance[v] + w < distance[u]) {
                distance[u] = distance[v] + w;
                parent[u] = v;
            }
        }
    }

    printf("\n[Bellman-Ford]\n");
    printf("최단거리: %d분\n", distance[end]);
    printf("최단경로: ");
    printPath(parent, start, end);
    printf("\n");
}

int main(void)
{
    int start, end;
    int menu;

    makeCampusGraph();

    printf("[캠퍼스 가중치 그래프 알고리즘 비교]\n");
    printf("정점 수: %d개\n", V);
    printf("간선 수: %d개\n", edgeCount);

    printf("\n========== MST 알고리즘 비교 ==========\n");
    kruskalMST();
    primMST();
    boruvkaMST();

    while (1) {
        printf("\n========== 최단거리 알고리즘 비교 ==========\n");
        printf("1. 최단경로 검색\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &menu);

        if (menu == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }

        if (menu != 1) {
            printf("잘못된 선택입니다.\n");
            continue;
        }

        printVertexMenu();

        printf("\n출발지 번호 입력: ");
        scanf("%d", &start);

        printf("도착지 번호 입력: ");
        scanf("%d", &end);

        if (start < 0 || start >= V || end < 0 || end >= V) {
            printf("잘못된 입력입니다.\n");
            continue;
        }

        if (start == end) {
            printf("출발지와 도착지가 같습니다.\n");
            continue;
        }

        printf("\n출발지: %s\n", names[start]);
        printf("도착지: %s\n", names[end]);

        dijkstra(start, end);
        floydWarshall(start, end);
        bellmanFord(start, end);
    }

    return 0;
}
