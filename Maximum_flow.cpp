#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Structure to represent a directed edge in the flow network
struct Edge {
    int to;
    int capacity;
    int flow;
    Edge(int t, int c) : to(t), capacity(c), flow(0) {}
};

// Function to add an edge to the graph
void addEdge(vector<vector<Edge>>& graph, int from, int to, int capacity) {
    graph[from].emplace_back(to, capacity);
    graph[to].emplace_back(from, 0); // Add a residual edge with initial flow 0
}

// Function to find augmenting paths using BFS
bool findAugmentingPath(vector<vector<Edge>>& graph, vector<int>& parent, int source, int sink) {
    fill(parent.begin(), parent.end(), -1);
    parent[source] = source;

    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (const Edge& edge : graph[current]) {
            if (parent[edge.to] == -1 && edge.capacity > edge.flow) {
                parent[edge.to] = current;
                if (edge.to == sink) {
                    return true;
                }
                q.push(edge.to);
            }
        }
    }

    return false;
}

// Ford-Fulkerson algorithm for maximum flow
int fordFulkerson(vector<vector<Edge>>& graph, int source, int sink) {
    int maxFlow = 0;
    vector<int> parent(graph.size());

    while (findAugmentingPath(graph, parent, source, sink)) {
        int pathFlow = INF;

        // Find the minimum capacity along the augmenting path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (const Edge& edge : graph[u]) {
                if (edge.to == v) {
                    pathFlow = min(pathFlow, edge.capacity - edge.flow);
                    break;
                }
            }
        }

        // Update flows along the augmenting path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (Edge& edge : graph[u]) {
                if (edge.to == v) {
                    edge.flow += pathFlow;
                    for (Edge& revEdge : graph[v]) {
                        if (revEdge.to == u) {
                            revEdge.flow -= pathFlow;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    int numNodes = 6;
    int source = 0;
    int sink = numNodes - 1;

    vector<vector<Edge>> graph(numNodes);

    // Adding edges to the graph
    addEdge(graph, 0, 1, 10);
    addEdge(graph, 0, 2, 5);
    addEdge(graph, 1, 2, 15);
    addEdge(graph, 1, 3, 5);
    addEdge(graph, 2, 3, 10);
    addEdge(graph, 2, 4, 10);
    addEdge(graph, 3, 4, 5);
    addEdge(graph, 3, 5, 15);
    addEdge(graph, 4, 5, 10);

    int maxFlow = fordFulkerson(graph, source, sink);

    cout << "Maximum Flow: " << maxFlow << endl;

    return 0;
}