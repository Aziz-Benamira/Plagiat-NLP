#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

class WeightedGraph {
    int numVertices;
    vector<vector<pair<int, int>>> adjacencyList;

public:
    WeightedGraph(int vertices) : numVertices(vertices), adjacencyList(vertices) {}

    void addConnection(int node1, int node2, int weight) {
        adjacencyList[node1].emplace_back(node2, weight);
        adjacencyList[node2].emplace_back(node1, weight); // Bidirectional graph
    }

    vector<int> shortestPath(int startNode) {
        vector<int> minDistances(numVertices, numeric_limits<int>::max());
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap;

        minDistances[startNode] = 0;
        minHeap.emplace(0, startNode);

        while (!minHeap.empty()) {
            int currentDistance = minHeap.top().first;
            int currentVertex = minHeap.top().second;
            minHeap.pop();

            if (currentDistance > minDistances[currentVertex]) continue;

            for (auto& neighbor : adjacencyList[currentVertex]) {
                int adjacentNode = neighbor.first;
                int edgeWeight = neighbor.second;

                if (currentDistance + edgeWeight < minDistances[adjacentNode]) {
                    minDistances[adjacentNode] = currentDistance + edgeWeight;
                    minHeap.emplace(minDistances[adjacentNode], adjacentNode);
                }
            }
        }
        return minDistances;
    }
};

int main() {
    WeightedGraph graph(6);
    graph.addConnection(0, 1, 4);
    graph.addConnection(0, 2, 4);
    graph.addConnection(1, 3, 3);
    graph.addConnection(2, 3, 2);
    graph.addConnection(3, 4, 6);
    graph.addConnection(4, 5, 5);

    int startVertex = 0;
    vector<int> distances = graph.shortestPath(startVertex);

    cout << "Shortest path distances from vertex " << startVertex << ":\n";
    for (int i = 0; i < distances.size(); ++i) {
        cout << "Vertex " << i << ": " << distances[i] << '\n';
    }

    return 0;
}
