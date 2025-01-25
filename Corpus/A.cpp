#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

class Graph {
    int vertices;
    vector<vector<pair<int, int>>> adjList;

public:
    Graph(int v) : vertices(v), adjList(v) {}

    void addEdge(int u, int v, int weight) {
        adjList[u].emplace_back(v, weight);
        adjList[v].emplace_back(u, weight); // Remove for directed graphs
    }

    vector<int> dijkstra(int start) {
        vector<int> distances(vertices, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

        distances[start] = 0;
        pq.emplace(0, start);

        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int currentNode = pq.top().second;
            pq.pop();

            if (currentDist > distances[currentNode]) continue;

            for (auto& neighbor : adjList[currentNode]) {
                int nextNode = neighbor.first;
                int weight = neighbor.second;

                if (currentDist + weight < distances[nextNode]) {
                    distances[nextNode] = currentDist + weight;
                    pq.emplace(distances[nextNode], nextNode);
                }
            }
        }
        return distances;
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 3, 3);
    g.addEdge(2, 3, 2);
    g.addEdge(3, 4, 6);
    g.addEdge(4, 5, 5);

    int startNode = 0;
    vector<int> distances = g.dijkstra(startNode);

    cout << "Shortest distances from node " << startNode << ":\n";
    for (int i = 0; i < distances.size(); ++i) {
        cout << "Node " << i << ": " << distances[i] << '\n';
    }

    return 0;
}
