/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding the cheapest distance and path from a node to all other nodes.

Implemented algorithm: Dijkstra's algorithm using a priority queue was used. The priority queue stores nodes
where Node.index represents the destination node and Node.total_cost represents the total cheapest known 
weight from start to this destination. The total_cost for each node is initialized to a number that is larger than 
the max_weight of an edge (in other words practically infinity). For each node in the priority queue the algorithm 
loops through all its edges and adds nodes if their total cost is cheaper than what was previously known. Stale 
nodes in the priority queue are not processed to save some time. Once the priority queue is 
empty all edges that lead to cheaper paths have been processed.

Time complexity: O(E*logV), where E is the number of edges and V is the number of vertices. This is because in 
the worst case add all edges to the priority queue. In this scenario each time we inspect an edge it leads to a 
cheaper path. Pushing E number of times into a priority queue has the time complexity O(E*logE). Since however 
E <= V^2 and O(logV^2) = O(2logV) = O(logV), we can simplify the time complexity to O(E*logV).

Space complexity: O(V+E) because we use an adjacency list.

Use: Input the number of nodes, the number of vertices, the number of queries and the starting node index in that order.
Then for each vertex input the start node, the end node and the weight. Notice that these vertices are directed. Lastly
input the queries to get the optimal distance to a index. The parent vector can be used to find the optimal path by iterating 
from the destination node back to the start node. The static overflow variable can be modified depending on the problem 
limits. The overflow must be larger than the total weight in any path the algorithm iterates over (to be safe set: 
overflow > number_of_nodes * max_edge_weight). unsigned long long could also be used instead of int if overflow
needs to be much larger.
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
    int total_cost;
    inline static int overflow = 100000000;
};

pair<vector<int>, vector<int>> shortest_path(vector<vector<Edge>> const& graph, int start)
{
    int number_of_nodes {static_cast<int>(graph.size())};

    if (start < 0 || start >= number_of_nodes )
    {
        throw invalid_argument("Start can't be less than zero or larger than the number of nodes given in graph.");
    }

    vector<int> parent (number_of_nodes, -1);
    vector<int> shortest_distances (number_of_nodes, Node::overflow);
    priority_queue<Node, vector<Node>, greater<Node>> cheaptest_node {};

    shortest_distances[start] = 0;
    parent[start] = start;
    cheaptest_node.push(Node{start, 0});
    while (!cheaptest_node.empty())
    {
        Node current {cheaptest_node.top()};
        cheaptest_node.pop();
        int distance_from_start {current.total_cost};

        if (distance_from_start > shortest_distances[current.index])
        {
            // We don't need to recheck an node via a more expensive path
            continue;
        }

        // From current node check each edge to find cheaper paths to other nodes
        // If found update routes and add the node with the updated total cost to the priority queue
        for(Edge const& e : graph[current.index])
        {
            if (distance_from_start + e.weight > Node::overflow || distance_from_start > INT_MAX - e.weight)
            {
                throw invalid_argument("The given graph is produces a distance that is larger than Edge::overflow");
            }
            if (distance_from_start + e.weight < shortest_distances[e.node])
            {
                shortest_distances[e.node] = distance_from_start + e.weight;
                parent[e.node] = current.index;

                cheaptest_node.push(Node{e.node, distance_from_start + e.weight});
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
        vector<int> const& shortest_distances {result.first};
        //vector<int> const& parent {result.second};

        // Answering queries
        for (int i{}; i < queries; i++)
        {
            int query;
            cin >> query;
            if (shortest_distances[query] >= Node::overflow)
            {
                cout << "Impossible\n";
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