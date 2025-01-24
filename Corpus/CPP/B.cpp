#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

class DijkstraGraph {
    int vCount;
    vector<vector<pair<int, int>>> adj;

public:
    DijkstraGraph(int v) : vCount(v) {
        adj.resize(v);
    }

    void linkNodes(int a, int b, int wt) {
        adj[a].push_back({b, wt});
        adj[b].push_back({a, wt});
    }

    vector<int> computeDijkstra(int source) {
        vector<int> dist(vCount, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto top = pq.top();
            pq.pop();
            int curNode = top.second, curDist = top.first;

            if (curDist > dist[curNode]) continue;

            for (auto& n : adj[curNode]) {
                int neighbor = n.first, cost = n.second;
                if (curDist + cost < dist[neighbor]) {
                    dist[neighbor] = curDist + cost;
                    pq.push({dist[neighbor], neighbor});
                }
            }
        }
        return dist;
    }
};

int main() {
    DijkstraGraph g(6);
    g.linkNodes(0, 1, 4);
    g.linkNodes(0, 2, 4);
    g.linkNodes(1, 3, 3);
    g.linkNodes(2, 3, 2);
    g.linkNodes(3, 4, 6);
    g.linkNodes(4, 5, 5);

    vector<int> result = g.computeDijkstra(0);

    cout << "Resulting distances:\n";
    for (int i = 0; i < result.size(); i++) {
        cout << "Node " << i << " -> Distance: " << result[i] << endl;
    }

    return 0;
}
