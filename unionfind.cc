/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Meging elements into sets and checking if two elements are in the same set.

Implemented algorithm: Sets are represented as trees where elements point to the root instead of the 
leaf nodes. The root is the representative node and two elements with the same representative are therefore 
in the same set. To effefiently find representatives of nodes the tree should be kept as shallow as possible.
This is done via two optimization methods. (1) mergeing smaller trees into larger ones. (2) Flattening out
nodes during searches (see find_rep for more info).

Time complexity: O(inverse_ackerman(n)) which in practice is O(1) for both operations.

Use: Input the number of elements in the union-find datastructure followed by the number of operations. For
each operation input "= <a> <b> for mergeging a and b and "? <a> <b>" for checking if a and b are in the same
set. If they are in the same set "yes" is printed and if not "no" is printed.
*/

#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// A class that efficiently unions sets finds out if two elements are in the same set
// Sets are represented as trees where elements point to the root instead of the leaf nodes.
// The root is the representative node and two elements with the same representative are therefore in the same set.
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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int length{}, operations{};
    cin >> length >> operations;
    UnionFind uf{length};
    for (int i{}; i < operations; i++)
    {
        char op{};
        int lhs{}, rhs{};
        cin >> op >> lhs >> rhs;

        if (op == '?')
        {
            if (uf.same(lhs, rhs))
            {
                cout << "yes\n";
            }
            else
            {
                cout << "no\n";
            }
        }
        else if (op == '=')
        {
            uf.merge(lhs, rhs);
        }
    }
    return 0;
}