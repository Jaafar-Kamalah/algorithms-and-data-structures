/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding the a tree between all nodes where the cost of each edge is minimal.

Implemented algorithm: Kruskal's algorithm.

Time complexity: O(E*log(E)), where E is the number of edges. This is because we sort a container of size E and that
incurs the dominating factor. The loop inside mst() incurs a cost of O(E * inverse_ackerman(V)), where V is the number 
of nodes, which in practice is less than O(E*log(E)). Moreover, sorting the mst container incurs a cost of O(V * log(V)) 
which is always less than O(E*log(E)) since we end the algortihm early when V > E-1.

Space complexity: O(E), E is the number of edges. This is because we store all the  edges in a edge list which incurs O(E). 
We also initialize a unionfind container of size V, where V is the number of nodes. However our algorithm returns early if
V > E-1, O(E + V) = O(E).

Use: Input the number of nodes and the number of edges. For each undirected edge input:  node1, node2 and weight. The
algorithm will produce the cost of the msp and all the edges included in lexicographic order, where node1 is always
smaller than node2. If there is no solution the size of the msp edge list will be smaller than the number of nodes - 1.
If multiple solutions exist one of the vable solutions will be returned.
*/

#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>
#include <climits>
#include <algorithm>

using namespace std;

class UnionFind
{
    public:
    UnionFind(int l) : length{l}, parent{}, size{}
    {
        if (length < 0)
        {
            throw invalid_argument("UnionFind length can not be negative");
        }
        for (int i{}; i < length; i++)
        {
            parent.push_back(i);
            size.push_back(1);
        }
    }

    // Find the representative of a set by recursively searching parent[]
    // Two sets with the same representative are in the same set
    int find_rep(int a)
    {
        if (a >= length || a < 0)
        {
            throw invalid_argument("Element \"" + to_string(a) + "\" does not exist. The valid range is 0.." + to_string(length-1));
        }

        if (parent[a] == a)
        {
            // Base case for recursive function
            // Representative nodes have themselves as parents
            return a;
        }

        // We make lookup for nodes between a and the representative node shorter during the search
        // This is done by assigning those nodes as direct children of the representative node
        int rep {find_rep(parent[a])};
        parent[a] = rep;
        return rep;
    }

    bool same(int a, int b)
    {
        return find_rep(a) == find_rep(b);
    }

    // merge/union two sets by assigning the one of the sets representative as parent for the other sets representative
    void merge(int a, int b)
    {
        if (a >= length || a < 0)
        {
            throw invalid_argument("Element \"" + to_string(a )+ "\" does not exist. The valid range is 0.." + to_string(length-1));
        }
        else if (b >= length || b < 0)
        {
            throw invalid_argument("Element \"" + to_string(b) + "\" does not exist. The valid range is 0.." + to_string(length-1));
        }

        // We attach the smaller set to the larger set to minimize depth of the tree
        // More shallow trees allow us get the tree representative faster on average
        int a_rep{find_rep(a)};
        int b_rep{find_rep(b)};

        if (a_rep != b_rep)
        {
            // a and b are already in same set if they have the same representative
            
            if (size[a_rep] > size[b_rep])
            {
                parent[b_rep] = a_rep;
                size[a_rep] += size[b_rep];
            }
            else
            {
                parent[a_rep] = b_rep;
                size[b_rep] += size[a_rep];
            }
        }
    }

    private:
    int length;
    vector<int> parent; 
    vector<int> size;
};

struct Edge
{
    int node1;
    int node2;
    int weight;
};

pair<int, vector<Edge>> mst(vector<Edge> graph, int nodes)
{

    vector<Edge> mst {};
    int total_weight {};

    if (graph.size() < nodes-1)
    {
        // It is always impossible to build a spanning tree when there are less edges than nodes
        return {total_weight, mst};
    }

    sort(graph.begin(), graph.end(), [](Edge const& a, Edge const& b)
                                    {
                                        return a.weight < b.weight;
                                    });
    UnionFind uf{nodes};
    for (Edge e : graph)
    {
        if (!uf.same(e.node1, e.node2))
        {
            if (e.node2 < e.node1)
            {
                swap(e.node2, e.node1);
            } 
            mst.push_back(e);
            total_weight += e.weight;
            uf.merge(e.node1, e.node2);
        }
    }

    sort(mst.begin(), mst.end(), [](Edge const& a, Edge const& b)
                                    {
                                        if (a.node1 == b.node1)
                                        {
                                            return a.node2 < b.node2;
                                        }
                                        return a.node1 < b.node1;
                                    });
    return {total_weight, mst};
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    long unsigned nodes, edges;
    cin >> nodes >> edges;

    while (nodes != 0 || edges != 0)
    {
        // We use an edge list
        vector<Edge> graph(edges);

        // Populating graph
        for (unsigned long i{}; i < edges; i++)
        {
            int node1, node2, weight;
            cin >> node1 >> node2 >> weight;
            // The edges are bidirectional
            graph[i] = Edge{node1, node2, weight};
        }


        auto result {mst(graph, nodes)};
        int const& total_cost {result.first};
        vector<Edge> const& mst_edges {result.second};

        // Output the total cost for all edges in msp and all relevant edges
        if (mst_edges.size() != nodes-1)
        {
            // If a mst exists then there should be exactly (nodes - 1) chosen edges
            cout << "Impossible\n";
        }
        else
        {
            cout << total_cost << '\n';
            // mst() sorts the edge result in lexicographic order
            for (Edge const& e : mst_edges)
            {
                cout << e.node1 << ' ' << e.node2 << '\n';
            }
        }
        cin >> nodes >> edges;
    }

    return 0;
}