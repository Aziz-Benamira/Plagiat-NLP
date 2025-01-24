#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

class Graph {
    int nodes;
    vector<vector<pair<int, int>>> graph;

public:
    Graph(int n) : nodes(n), graph(n) {}

    void connect(int u, int v, int weight) {
        graph[u].push_back(make_pair(v, weight));
        graph[v].push_back(make_pair(u, weight)); // For undirected graph
    }

    void findShortestPaths(int start) {
        vector<int> distances(nodes, INT_MAX);
        distances[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int dist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            for (const auto& edge : graph[node]) {
                int neighbor = edge.first;
                int cost = edge.second;

                if (dist + cost < distances[neighbor]) {
                    distances[neighbor] = dist + cost;
                    pq.push(make_pair(distances[neighbor], neighbor));
                }
            }
        }

        cout << "Shortest paths from node " << start << ":\n";
        for (int i = 0; i < nodes; ++i) {
            cout << i << " -> " << distances[i] << "\n";
        }
    }
};

int main() {
    Graph g(6);
    g.connect(0, 1, 4);
    g.connect(0, 2, 4);
    g.connect(1, 3, 3);
    g.connect(2, 3, 2);
    g.connect(3, 4, 6);
    g.connect(4, 5, 5);

    g.findShortestPaths(0);
    return 0;
}
