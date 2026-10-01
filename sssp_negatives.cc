/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding the cheapest distance and path from a node to all other nodes where edges can be negative.

Implemented algorithm: The Bellman-Ford algorithm was used. In this algorithm we first iterate over all the edges
enough times and perform relaxations so that we get the optimal distance to all nodes not reachable by a negative cycle.
Nodes reachable by negative cycles are also relaxed but don't have the optimal distance yet because their optimal
distance is negative infinity (which would take infinite iterations). This first part can not be solved with 
Dijkestra's algorithm because it would get stuck in a infinite loop once a negative cycle is detected. Instead we 
use a arbitary order of the edges and iterate over that order V-1 times. In the second part we iterate over all the 
edges the same as we did in the first part. Instead of performing relaxations however we instead detect nodes reachable
by negative cycles. This is possible because we know that after the first part we have the optimal distances to all 
nodes not reachable by a negative cycle, so if we in the second part find a better path for a node it has to be
reachable by a negative cycle and should therefore be assigned negative infinity.

Time complexity: O(E*V), where E is the number of edges and V is the number of vertices. This is because we loop over all
edges V-1 times.

Space complexity: O(V+E) because we use an adjacency list.

Use: Input the number of nodes, the number of vertices, the number of queries and the starting node index in that order.
Then for each vertex input the start node, the end node and the weight. Notice that these vertices are directed. Lastly
input the queries to get the optimal distance to a index. The parent vector can be used to find the optimal path by iterating 
from the destination node back to the start node. The static INF variable can be modified depending on the problem 
limits.
*/

#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include <climits>

using namespace std;

struct Edge
{
    int node;
    int weight;
};

struct Node
{
    bool operator>(Node const& rhs) const
    {
        return total_cost > rhs.total_cost;
    }

    int index;
    long long total_cost;
    inline static long long INF = 1000000000000;
};

pair<vector<long long>, vector<int>> shortest_path(vector<vector<Edge>> const& graph, int start)
{
    int number_of_nodes {static_cast<int>(graph.size())};

    if (start < 0 || start >= number_of_nodes )
    {
        throw invalid_argument("Start can't be less than zero or larger than the number of nodes given in graph.");
    }

    vector<int> parent (number_of_nodes, -1);
    vector<long long> shortest_distances (number_of_nodes, Node::INF);

    shortest_distances[start] = 0;
    parent[start] = start;

    // Iteration 1:
    // Iterate over all edges N-1 times, where N is the number of nodes and update each edges total_cost if a cheaper one is found.
    // We need to iterate N-1 times to ensure that all the shortest_distances are optimal because the edge iteration order is random.

    // Iteration 2:
    // Iterate over all edges N-1 times again, but this time because we know that shortest_distances are optimal, if we find a cheaper 
    // path it is because of a negative cycle. We therefore assign that node -Infinity. We need to iterate N-1 times to ensure that
    // all nodes reachable by a negative cycle also are assigned -Infinity (not just the nodes in the negative cycle).
    for (int i{1}; i <= 2; i++)
    {
        for (int j{}; j < number_of_nodes-1; j++)
        {
            // Iterate over all edges
            for (int from{}; from < number_of_nodes; from++)
            {
                for (Edge const& to : graph[from])
                {
                    if (shortest_distances[from] != Node::INF)
                    {
                        if ((to.weight < 0 && shortest_distances[from] < LLONG_MIN - to.weight) ||
                            (to.weight > 0 && shortest_distances[from] > LLONG_MAX - to.weight) ||
                            (i == 1 && shortest_distances[from] + to.weight > Node::INF) ||
                            (i == 1 && shortest_distances[from] + to.weight < -Node::INF))
                        {
                            throw invalid_argument("The given graph is produces a distance that is larger than Node::INF or smaller than -Node::INF.");
                        }

                        if (shortest_distances[from] + to.weight < shortest_distances[to.node])
                        {
                            if (i == 1)
                            {
                                shortest_distances[to.node] = shortest_distances[from] + to.weight;
                            }
                            else
                            {
                                shortest_distances[to.node] = -Node::INF;
                            }
                            parent[to.node] = from;
                        }
                    }
                }
            }
        }
    }

    return {shortest_distances, parent};
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int nodes, edges, queries, starting_node;
    cin >> nodes >> edges >> queries >> starting_node;

    while (nodes != 0 || edges != 0 || queries != 0 || starting_node != 0)
    {
        // The graph is pretty sparse (max 30 k edges with max 10 k nodes)
        // So we use a adjacency list to represent the graph
        vector<vector<Edge>> graph(nodes);

        // Populating graph
        for (int i{}; i < edges; i++)
        {
            int from, to, weight;
            cin >> from >> to >> weight;
            graph[from].push_back(Edge{to, weight});
        }


        auto result {shortest_path(graph, starting_node)};
        vector<long long> const& shortest_distances {result.first};
        //vector<int> const& parent {result.second};

        // Answering queries
        for (int i{}; i < queries; i++)
        {
            int query;
            cin >> query;
            if (shortest_distances[query] >= Node::INF)
            {
                cout << "Impossible\n";
            }
            else if (shortest_distances[query] <= -Node::INF)
            {
                cout << "-Infinity\n";
            }
            else
            {
                cout << shortest_distances[query] << "\n";
            }
        }
        cout << "\n";
        cin >> nodes >> edges >> queries >> starting_node;
    }

    return 0;
}