#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const int V = 6; // Number of nodes in the graph (including source and sink)

// A utility function to perform Breadth-First Search (BFS)
bool bfs(int residualGraph[V][V], int source, int sink, int parent[]) {
    bool visited[V] = {false};
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[sink];
}

// Ford-Fulkerson algorithm to find the maximum flow
int fordFulkerson(int graph[V][V], int source, int sink) {
    int u, v;
    int residualGraph[V][V];

    // Initialize residual graph to be the same as the original graph
    for (u = 0; u < V; u++) {
        for (v = 0; v < V; v++) {
            residualGraph[u][v] = graph[u][v];
        }
    }

    int parent[V];
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    int graph[V][V] = {
        {0, 10, 15, 0, 0, 0},
        {0, 0, 5, 15, 10, 0},
        {0, 0, 0, 0, 15, 10},
        {0, 0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0, 0}
    };

    int source = 0;
    int sink = 5;

    int maxFlow = fordFulkerson(graph, source, sink);
    cout << "Maximum Flow: " << maxFlow << endl;

    return 0;
}
