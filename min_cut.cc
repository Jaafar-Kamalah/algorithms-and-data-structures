/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Given a directed weighted graph, a source node and a sink node, finding a subset U where the weight of outgoing 
edges  between vertices of U and outside of U is minimal. Moreover, U has to contain the source node and can not contain the 
sink node.

Implemented algorithm: Using the Edmond-Karp algorithm and calculating which nodes are reachable from source in the residual 
graph. The Edmonds-Karp algorithm is an implementation of the Ford-Fulkerson method which repeatedly finds augmenting paths 
and augments flow until no more augmenting paths can be found. An augmenting path is a path of edges from the source to the 
sink where the available capacity is greater than zero. Edges consist of forward edges and backward edges. If a forward edge 
is between nodes A and B, then a backward edge is between B and A. Augmenting flow means increasing all edges in a augmenting 
path with the bottleneck of available capacity and decreasing all opposite edges (backward edges is the opposite of forward 
edges and vice versa). The resulting residual graph is a graph containing all the forward and backward edges where flow has
fully been augmented.

Time complexity: O(E^2 * V), where E is the total number of edges and V is the total number of nodes. This is because at 
most BFS is ran O(V * E) times and each BFS has the time complexity of O(E). The reason why BFS is ran at most O(V * E) times 
is because each edge can at most be saturated V times, since each saturation pushes the edge outward in the BFS layer.

Space complexity: O(V + E), where E is the total number of edges and V is the total number of nodes. This is because the
adjacency list stores one vector per vertex O(V) and stores all edges O(E), which gives us O(V+E). The parent container
and reachable_from_src container are smaller.

Use: Input the number of nodes , the number of edges, the source node and the sink node. Then for each directed edge input
node1, node2 and the capacity/weight. The algorithm will output the number of nodes in the subset U. Then it will output all 
nodes inside of U. The algorithm can not handle flow graphs where the max_flow exceeds the size of a long long or where the 
total number of nodes or edges exceeds the size of a int.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

using ll = long long;

struct Edge
{
    Edge(int f, int t, ll c, bool fo) : from{f}, to{t}, capacity{c}, flow{0}, forward{fo}, opposite{nullptr}
    {}

    int from;
    int to;
    ll capacity;
    ll flow;
    bool forward;
    Edge* opposite;
};

struct Graph
{
    int nodes;
    // We use an adjacency list because we want fast lookup of neighbours for BFS
    vector<vector<Edge*>> adj;
};

ll max_flow(Graph & G, int const& src, int const& snk)
{
    ll maxflow{};

    while(true)
    {
        // Use BFS to find an augmenting path and encode it into parent 
        vector<Edge*> parent(G.nodes, nullptr); // nullptr represents unvisited edges
        queue<int> q{}; 
        q.push(src);

        while (!q.empty() && parent[snk] == nullptr)
        {
            int curr = q.front();
            q.pop();

            // Add all unvisited and flowable neighours of curr that don't lead back to source to the queue
            for (Edge* neighbour : G.adj[curr])
            {
                if (parent[neighbour->to] == nullptr && neighbour->to != src && neighbour->capacity > neighbour->flow)
                {
                    parent[neighbour->to] = neighbour;
                    q.push(neighbour->to);
                }
            }
        }

        if (parent[snk] == nullptr)
        {
            break; // No augmenting path found
        }

        // Find bottleneck in the augmenting path
        ll bottleneck{LLONG_MAX};
        for (Edge* i = parent[snk]; i != nullptr; i = parent[i->from])
        {
            bottleneck = min(bottleneck, i->capacity - i->flow);
        }

        // Augment the flow of the augmenting path
        for (Edge* i = parent[snk]; i != nullptr; i = parent[i->from])
        {
            i->flow += bottleneck;
            i->opposite->flow -= bottleneck;
        }
        maxflow += bottleneck;
    }

    return maxflow;
}

vector<int> min_cut(Graph & G, int const& src, int const& snk)
{
    max_flow(G, src, snk);

    // Run BFS to find all reachable nodes from src
    vector<bool> visited(G.nodes, false);

    queue<int> q;
    q.push(src);
    visited[src] = true;

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        // Push all neighours of curr in the queue
        for (Edge* neighbour : G.adj[curr])
        {
            if (!visited[neighbour->to] && neighbour->flow < neighbour->capacity)
            {
                visited[neighbour->to] = true;
                q.push(neighbour->to);
            }
        }
    }

    vector<int> reachable{};
    for (int i{}; i < G.nodes; i++)
    {
        if (visited[i])
            reachable.push_back(i);
    }

    return reachable;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int nodes, edges, src, snk;
    cin >> nodes >> edges >> src >> snk;

    Graph G{};
    G.nodes = nodes;
    G.adj = vector<vector<Edge*>>(nodes);

    // Populating graph
    for (int i{}; i < edges; i++)
    {
        // Add the directed edges and their corresponding backward edges
        int from, to;
        ll capacity;
        cin >> from >> to >> capacity;

        Edge* forward {new Edge{from, to, capacity, true}};
        Edge* backward {new Edge{to, from, 0, false}};
        forward->opposite = backward;
        backward->opposite = forward;

        G.adj[from].push_back(forward);
        G.adj[to].push_back(backward);
    }

    vector<int> reachable_from_src {min_cut(G, src, snk)};

    //Output handling
    cout << reachable_from_src.size() << "\n";
    for(int v : reachable_from_src)
    {
        cout << v << "\n";
    }

    return 0;
}