/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: In a directed graph find a path that traverses all edges exactly once if such a path exists.

Implemented algorithm: Hierholzer's algorithm. Uses DFS to until a dead end is reached, then backtracks one 
step and tries to search for unvisited edges again. For each backtrack the solution is built (in reverse). 
The DFS starts on a start node, which is a node where outgoing_edge - ingoing_edge == 1. If no such node 
exists then any node with an outgoing edge can be the start node. There are moreover several conditions used 
to determine if a eulerian path is impossible (see code for details).

Time complexity: O(V + E), where V is the total number of nodes and E is the total number of edges. The time
dominating section is the first loop in the algorithm that records all the outgoing and igoing edges. In 
this section, all nodes are iterated over in the outer loop and all edges are iterated over in the inner 
loop, giving O(V + E). The DFS is only O(E).

Space complexity: O(V + E), because of the adjacency list which dominates in memory usage.

Use: Input the total number of nodes (<INT_MAX), the total number of edges (<INT_MAX) and followed by a from
and to node index for each edge (index starts on 0). The program will output "Impossible" if no eulerian 
path exists or the sequence of node indexes that are visited when following the path.
*/

#include <iostream>
#include <stack>
#include <vector>
#include <deque>


using namespace std;

void eulerian_dfs(int at, vector<vector<int>> const& graph, vector<int> & out, deque<int> & path)
{
    while (out[at] != 0)
    {
        int next_node {graph[at][--out[at]]};
        eulerian_dfs(next_node, graph, out, path);
    }
    path.push_front(at);
}

deque<int> eulerian_path(vector<vector<int>> const& graph)
{
    // Record how many outgoing and ingoing edge each node has
    vector<int> in(graph.size());
    vector<int> out(graph.size());
    int no_of_edges{};
    for(size_t i{}; i < graph.size(); i++)
    {
        for(size_t j{}; j < graph[i].size(); j++)
        {
            out[i]++;
            in[graph[i][j]]++;
            no_of_edges++;
        }
    }

    // Calculate if a eulerian path is possible:
    // *At most one node has one more outgoing edge than ingoing edge (start-node)
    // *At most one node has one more ingoing edge than outgoing edge (end-node)
    // *All other nodes have an equal amount of ingoing and outgoing edges
    int start{-1};
    int alt_start{0};
    int end{-1};
    for(size_t i{}; i < graph.size(); i++)
    {
        if (in[i] - out[i] > 1 || out[i] - in[i] > 1 ||
            (start != -1 && out[i] - in[i] == 1) || (end != -1 && in[i] - out[i] == 1))
        {
            return{};
        }
        else if (out[i] - in[i] == 1)
        {
            start = i;
        }
        else if (in[i] - out[i] == 1)
        {
            end = i;
        }

        // If we never find a definitive start node than we can use
        // alt_start (node 0 or any node with a outgoing edge)
        if (alt_start == 0 && out[i] > 0)
            alt_start = i;
    }

    if (start == -1)
        start = alt_start;

    // A eulerian path is possible as long as the edgse are not disjoint.
    // Find the path by:
    // 1. Running DFS until stuck
    // 2. Add the node that DFS got stuck on to the solution
    // 3. Backtrack and repeat from step 1
    deque<int> path{};
    eulerian_dfs(start, graph, out, path);

    // If the path is not as long as the number of edges+1 we know the edges are disjoint.
    if (path.size() != no_of_edges + 1)
        return {}; // Return impossuble

    return path;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int no_of_nodes, no_of_edges;
    cin >> no_of_nodes >> no_of_edges;

    while (no_of_nodes != 0 || no_of_edges != 0)
    {
        // An adjacency list is used because fast access to neighours is needed
        vector<vector<int>> graph(no_of_nodes);

        // Populating the graph
        for (int i{}; i < no_of_edges; i++)
        {
            int from, to;
            cin >> from >> to;
            graph[from].push_back(to);
        }

        deque<int> path {eulerian_path(graph)};
        // Output handling
        if (path.empty())
        {
            cout << "Impossible";
        }
        else
        {
            // Printing path
            for (int n : path)
            {
                cout << n << ' ';
            }
        }
        cout << "\n" << endl;
        cin >> no_of_nodes >> no_of_edges;
    }

    return 0;
}