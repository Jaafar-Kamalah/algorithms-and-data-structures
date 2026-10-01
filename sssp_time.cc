/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding the cheapest distance and path from a node to all other nodes.

Implemented algorithm: Dijkstra's algorithm using a priority queue was used. The priority queue stores nodes
where Node.index represents the destination node and Node.total_cost represents the total cheapest known 
weight from start to this destination. The total_cost for each node is initialized to a number that is larger than 
the max_weight of an edge (in other words practically infinity). For each node in the priority queue the algorithm 
loops through all its edges and adds nodes if their total cost is cheaper than what was previously known. Stale 
nodes in the priority queue are not processed to save some time. Once the priority queue is empty all edges that 
lead to cheaper paths have been processed. Calculating the total_cost of a destination includes summing up the 
total_cost of the current node, the time_cost of the edge and the waiting time needed to access the edge, 

Time complexity: O(E*logV), where E is the number of edges and V is the number of vertices. This is because in 
the worst case add all edges to the priority queue. In this scenario each time we inspect an edge it leads to a 
cheaper path. Pushing E number of times into a priority queue has the time complexity O(E*logE). Since however 
E <= V^2 and O(logV^2) = O(2logV) = O(logV), we can simplify the time complexity to O(E*logV).

Space complexity: O(V+E) because we use an adjacency list.

Use: Input the number of nodes, the number of vertices, the number of queries and the starting node index in that order.
Then for each vertex input the start node, the end node, the start_time, the period and the time_cost. Notice that these 
vertices are directed. Lastly input the queries to get the optimal time to a index. The parent vector can be used to find 
the optimal path by iterating from the destination node back to the start node. The static overflow variable can be modified 
depending on the problem limits. The overflow must be larger than the total cost in any path the algorithm iterates over 
(to be safe set: overflow > number_of_nodes * (max_time_cost + max_wait_time). unsigned long long could also be used instead 
of int if overflow needs to be much larger.
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
    int start_time;
    int period;
    int time_cost;
};

struct Node
{
    bool operator>(Node const& rhs) const
    {
        return total_cost > rhs.total_cost;
    }

    int index;
    int total_cost;
    inline static int overflow = 1000000000;
};

pair<vector<int>, vector<int>> shortest_path(vector<vector<Edge>> const& graph, int start)
{
    int number_of_nodes {static_cast<int>(graph.size())};

    if (start < 0 || start >= number_of_nodes )
    {
        throw invalid_argument("The given graph is produces a duration that is larger than Edge::overflow");
    }

    vector<int> parent (number_of_nodes, -1);
    vector<int> shortest_duration (number_of_nodes, Node::overflow);
    priority_queue<Node, vector<Node>, greater<Node>> cheaptest_node {};

    shortest_duration[start] = 0;
    parent[start] = start;
    cheaptest_node.push(Node{start, 0});
    
    while (!cheaptest_node.empty())
    {
        Node current {cheaptest_node.top()};
        cheaptest_node.pop();
        int duration_from_start {current.total_cost};

        if (duration_from_start > shortest_duration[current.index])
        {
            // We don't need to recheck an node via a more expensive path
            continue;
        }

        // From current node check each edge to find cheaper paths to other nodes
        // If found upddate routes and add the node with the updated total cost to the priority queue
        for(Edge const& e : graph[current.index])
        {
            int period_factor {};
            if (duration_from_start > e.start_time)
            {
                if (e.period == 0)
                {
                    // Edge that was only available once was missed
                    continue;
                }
                // Calculate how many periods we need to wait until we can take the edge
                int lateness {duration_from_start - e.start_time};
                period_factor = lateness / e.period;
                if (lateness % e.period != 0)
                {
                    period_factor++;
                }
            }
            int wait_time {(e.start_time + e.period * period_factor) - duration_from_start};
            int new_duration {duration_from_start + wait_time + e.time_cost};

            if (new_duration > Node::overflow || duration_from_start > INT_MAX - (wait_time + e.time_cost))
            {
                throw invalid_argument("The given graph exceeds the Edge::overflow limit.");
            }
            if (new_duration < shortest_duration[e.node])
            {
                shortest_duration[e.node] = new_duration;
                parent[e.node] = current.index;

                cheaptest_node.push(Node{e.node, new_duration});
            }
        }
    }
    return {shortest_duration, parent};
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
            int from, to, start_time, period, time_cost;
            cin >> from >> to >> start_time >> period >> time_cost;
            graph[from].push_back(Edge{to, start_time, period, time_cost});
        }


        auto result {shortest_path(graph, starting_node)};
        vector<int> const& shortest_duration {result.first};
        //vector<int> const& parent {result.second};

        // Answering queries
        for (int i{}; i < queries; i++)
        {
            int query;
            cin >> query;
            if (shortest_duration[query] >= Node::overflow)
            {
                cout << "Impossible\n";
            }
            else
            {
                cout << shortest_duration[query] << "\n";
            }
        }
        cout << "\n";
        cin >> nodes >> edges >> queries >> starting_node;
    }

    return 0;
}