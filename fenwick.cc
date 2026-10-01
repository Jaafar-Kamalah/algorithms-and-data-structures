/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Handle range summation and addition in middle of container in O(logn)

Implemented algorithm: Sumation is calculated by iterating from the end_index to 0 where we in
each iteration flip the least significant set bit of the index in binary format. For example if
we wanted to sum up the values from 0 to 15 we add the sum of these indexes: (1111), (1110), (1100)
(1000). It is therefore clear that sumation has the time complexity O(logn). We could in practice
have O(1) for summation if we stored the sumation of 0 to N in index N, but then incrementing a 
value in the middle of the array would be inefficient. With our fenwick structure we can increment
a value to a index by iterating from that index to the length of the array where we in each iteration 
add the least significant set bit to the index. For example if we want to increment the value on index
(0001) and our length is (1111) we iterate over these indexes: (0001), (0010), (0100), (1000). It is 
also clear here that the time complexity is O(logn)

Time complexity: O(logn). Explanation under "Implemented algorithm"

Use: Input length of fenwick container, followed by how many operations to input. For each operation
input "+ <i> <a>" to add value a to index i or "? <i>" to sum all values from 0 to i, where i is not
included.
*/

#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// A class that efficiently (O(logn)) handles addition and range summation
class Fenwick
{
    public:
    Fenwick(int l) : length{l}, sums{}
    {
        if (length < 0)
        {
            throw invalid_argument("Fenwick length can not be negative");
        }

        // We assign length+1 elements and not length because element at index 0 is ignored
        sums.assign(length+1, 0);
    }

    void add(int index, int value)
    {
        index += 1; // Since we start on index 1
        while (index <= length)
        {
            sums[index] += value;
            // (index & -index) gives us the least significant set bit in binary format
            // For example if index = dec(6) = bin(0110) then (index & -index) = bin(0010)
            index += index & -index;
        }
    }

    // Sum of all elements from element 0 to end_index, where end_index is not included
    long long sum(int end_index)
    {
        long long sum{};
        // Since we start on index 1 we can include sums[end_index] in our sum
        while (end_index > 0)
        {
            sum += sums[end_index];
            end_index -= end_index & -end_index; // Flips the least significant set bit
        }
        return sum;
    }

    private:
    int length;
    vector<long long> sums;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int length{}, operations{};
    cin >> length >> operations;
    Fenwick fen{length};
    for (int i{}; i < operations; i++)
    {
        char op{};
        cin >> op;
        if (op == '?')
        {
            int end_index;
            cin >> end_index;
            cout << fen.sum(end_index) << "\n";
        }
        else if (op == '+')
        {
            int index, value;
            cin >> index >> value;
            fen.add(index, value);
        }
    }
    return 0;
}