/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Given a knapsack with limited capacity, maximize the total value of items that can be stored 
in it.

Implemented algorithm: A dynamic programming algorithm that uses a top-down approach with memoization. The
algorithm uses the state knapsack(n, capacity), where the value should be maximized given the knapsack
capacity using 0..n items. The relation between states is:
knapsack(n, capacity) = max(knapsack(n-1, capacity), value_of_item + knapsack(n-1, capacity-weight_of_item)).
Memoization is implemented using a 2D array that stores the value of each combination of item index and
capacity. After knapsack_rec fills the 2D array, the chosen indices are reconstructued by iterating the
container.

Time complexity: O(n*c), where n is the amount of items and c is the capacity. Without memoization the 
time complexity is O(2^n) since the state space concists of a binary tree with height n. With memoization
however each state is handled exactly once, and since there are only n*c unique combination this becomes 
the time complexity for knapsack_req. The reconstruction loop is only O(n).

Space complexity: O(n*c) The largest container is the 2D array used for memoization which stores each 
possible state and therefore has the exact size (n+1)(c+1). (+1 is to handle the states where no items are 
chosen and where the capacity is 0).

Use: Input one integer for the capacity of the knapsack and one integer for the number of items followed by
two integers for each item, where the first represents the value and the second represents the weight. The
output consists of one integer for the number of chosen items followed by all the indices of the chosen items.
*/

#include <iostream>
#include <vector>

using namespace std;

int knapsack_rec(int n, int capacity, vector<int> const& values, vector<int> const& weights, vector<vector<int>> & mem)
{
    if (n == 0 || capacity == 0)
    {
        // Base case: last item considered or knapsack is full
        return 0;
    }
    else if (mem[n][capacity] != -1)
    {
        // This subproblem has been calculated before
        return mem[n][capacity];
    }

    int capacity_if_chosen {capacity - weights[n-1]};
    int not_chosen {knapsack_rec(n-1, capacity, values, weights, mem)};
    

    if (capacity_if_chosen < 0)
    {
        mem[n][capacity] = not_chosen;
        return not_chosen;
    }
    else
    {
        int chosen {knapsack_rec(n-1, capacity_if_chosen, values, weights, mem)};
        chosen += values[n-1];

        mem[n][capacity] = max(chosen, not_chosen);
        return mem[n][capacity];
    }
}

vector<int> knapsack(int capacity, vector<int> const& values, vector<int> const& weights)
{
    int n {static_cast<int>(values.size())};
    vector<vector<int>> mem(n + 1, vector<int>(capacity + 1, -1));
    knapsack_rec(n, capacity, values, weights, mem);


    // Reconstructing the result
    vector<int> chosen_items;
    int c{capacity};
    for(int i{n}; i > 0; i--)
    {
        int weight_if_chosen {weights[i-1]};
        if (weight_if_chosen > c)
        {
            continue;
        }

        int value_if_chosen {mem[i-1][c-weight_if_chosen] + values[i-1]};
        int value_if_not_chosen {mem[i-1][c]};
        if (value_if_chosen > value_if_not_chosen)
        {
            chosen_items.push_back(i-1);
            c -= weight_if_chosen;
        }
    }

    return chosen_items;
}

int main()
{
    // Input handling
    int capacity;
    int number_of_items;
    while (cin >> capacity >> number_of_items)
    {
        vector<int> values{};
        vector<int> weights{}; 
        values.reserve(number_of_items);
        weights.reserve(number_of_items);

        for (int i{}; i < number_of_items; i++)
        {
            int value{};
            int weight{};
            cin >> value >> weight;
            values.push_back(value);
            weights.push_back(weight);
        }

        vector<int> indices {knapsack(capacity, values, weights)};

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