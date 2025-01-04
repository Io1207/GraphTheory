#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>

using namespace std;
bool bfs(vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) { 
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) return true; 
            }
        }
    }
    return false;
}

int edmondsKarp(vector<vector<int>>& capacity, int source, int sink) {
    int n = capacity.size();
    vector<vector<int>> residualGraph = capacity; 
    vector<int> parent(n); 
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    int n, m; 
    cout << "Enter number of vertices and edges: ";
    cin >> n >> m;

    vector<vector<int>> capacity(n, vector<int>(n, 0));

    cout << "Enter edges (u v capacity):" << endl;
    for (int i = 0; i < m; i++) {
        int u, v, cap;
        cin >> u >> v >> cap;
        capacity[u][v] = cap; 
    }

    int source, sink;
    cout << "give source and sink nodes: ";
    cin >> source >> sink;

    int maxFlow = edmondsKarp(capacity, source, sink);
    cout << "the maximum flow is: " << maxFlow << endl;
    return 0;
}
