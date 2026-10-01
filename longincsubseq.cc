/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding the longest increasing subsequence from a sequence of numbers.

Implemented algorithm: The algorithm is based on storing the smallest possible tail for a subsequence of 
length i in tails[i]. Therefore when we get a number that is not larger than all other previously seen numbers 
we overwrite the first number in tails which is larger or equal. A smaller tail overwrites a larger tail 
because it can lead to more outcomes and therefore possibly longer subsequences. By overwriting we invalidate 
tails as an actual subsequence and need a container that tracks the previous node of a inserted node to 
reconstruct the subsequence. 

Time complexity: O(n*log(n)). We do a binary search for each node. There are n nodes so the time complexity is 
O(n*log(n)). Reconstruction and reversing the result vector each take O(n) and are therefore not dominant.

Space complexity: O(n) because tails and prev can not be larger than seq.

Use: Input the number of elements in the sequence followed by the numbers in the sequence. The output consists
of the number in the longest increasing subsequence followed by the indices of the actual subsequence.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node
{
    bool operator<(Node const& rhs) const
    {
        return val < rhs.val;
    }

    int val;
    int index;
};

vector<int> longincsubseq(vector<Node> const& seq)
{
    if (seq.empty())
    {
        return {};
    }

    vector<Node> tails{};
    vector<int> prev(seq.size(), -1);

    for (Node const& n : seq)
    {

        if (tails.empty())
        {
            tails.push_back(n);
        }
        else if (n.val > tails.back().val)
        {
            prev[n.index] = tails.back().index;
            tails.push_back(n);
        }
        else
        {
            // Replace the first element that is not less than n using binary search
            auto it {lower_bound(tails.begin(), tails.end(), n)};
            *it = n;
            if (it != tails.begin())
            {
                prev[n.index] = (--it)->index;                
            }
        }
    }

    // Reconstruction
    vector<int> res;
    int index{tails.back().index};
    while(index != -1)
    {
        res.push_back(index);
        index = prev[index];
    }
    reverse(res.begin(), res.end());
    return res;
}

int main()
{
    // Input handling
    int seq_length;
    while (cin >> seq_length)
    {
        vector<Node> seq{};
        seq.reserve(seq_length);

        for (int i{}; i < seq_length; i++)
        {
            int val{};
            cin >> val;
            seq.push_back(Node{val,i});
        }

        vector<int> indices {longincsubseq(seq)};

        // Result displaying
        cout << indices.size() << "\n";
        for (int i : indices)
        {
            cout << i << " "; 
        }
        cout << "\n";
    }
    return 0;
}