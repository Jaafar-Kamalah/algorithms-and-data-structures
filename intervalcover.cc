/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Covering a numeric interval with minimal set of numeric intervals.

Implemented algorithm: A greedy algorithm that choses the interval with the largest second value among the 
intervals where first value is before a certain threshold. The threshold is initialized to the second value
of the intervall that should be covered and is updated when the interval that should be covered shrinks 
because new intervals are added to the result. Since the intervals are sorted at the start the algorithm
can solve the problem by iterating the intervals container, not needing to search the container for the 
next interval to evaluate and not ending up with a fragmented to_cover interval.

Time complexity: O(n*logn). The sorting at the beginning has the time complexity O(n*logn), everything after 
is O(n). Even though there are nested while loops the time complexity is not larger than O(n) because each
interval is handled exactly once and then removed.

Use: Input a interval to cover, followed by an int for how many available intervals to use for covering and
lastly all the available intervals. This outputs how many itnervals are used for covering followed by the 
indexes of the relevant indexes. For example, the input:
0 1
3
0 0.25
0.25 0.75
0.75 1

Gives the ouptut:
3
0 1 2
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Interval
{
    double start;
    double end;
    int index;
};

vector<int> cover(Interval const& to_cover, vector<Interval> intervals)
{
    if (intervals.empty())
    {
        return vector<int>{};
    }

    sort(intervals.begin(), intervals.end(), [](Interval const& lhs, Interval const& rhs)
                                             {
                                                return lhs.start > rhs.start;
                                             });
    double threshold{to_cover.start};
    vector<int> result{};
    while(true)
    {
        while (intervals.back().end < threshold)
        {
            intervals.pop_back();
        }

        if (intervals.empty() || intervals.back().start > threshold)
        {
            return vector<int>{};
        }

        Interval to_add{intervals.back()};
        intervals.pop_back();
        while(!intervals.empty() && intervals.back().start <= threshold)
        {
            if (to_add.end < intervals.back().end)
            {
                to_add = intervals.back();
            }
            intervals.pop_back();
        }

        result.push_back(to_add.index);
        threshold = to_add.end;
        
        if (threshold >= to_cover.end)
        {
            return result;
        }
        else if (intervals.empty())
        {
            return vector<int>{};
        }
    }
}

int main()
{
    Interval to_cover{};
    while (cin >> to_cover.start >> to_cover.end)
    {
        // Input handling
        if (to_cover.start > to_cover.end)
        {
            swap(to_cover.start, to_cover.end);
        }

        int intervals_available{};
        cin >> intervals_available;

        vector<Interval> intervals;
        intervals.reserve(intervals_available);

        for (int i{}; i < intervals_available; i++)
        {
            Interval interval;
            cin >> interval.start >> interval.end;
            if (interval.start > interval.end)
            {
                swap(interval.start, interval.end);
            }
            interval.index = i;
            intervals.push_back(interval);
        }


        vector<int> indices {cover(to_cover, intervals)};

        // Result displaying
        if (indices.empty())
        {
            cout << "impossible\n";
        }
        else
        {
            cout << indices.size() << "\n";
            for (int i : indices)
            {
                cout << i << " "; 
            }
            cout << "\n";
        }
    }
    return 0;
}