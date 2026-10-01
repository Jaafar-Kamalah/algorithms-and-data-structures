/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Finding a pattern in a text.

Implemented algorithm: Knuth-Morris-Pratt. The algorithm first prepares a longest prefix suffix table
where the longest prefix that matches a suffix is calculated for each prefix of the pattern. This 
table is then used to avoid restarting the search from the beginning of the pattern if possible when
a mismatch happens.

Time complexity: 
O(text_length + pattern_length). Computing lps takes O(pattern_length) while each charachter of the
text and pattern is handled a constant number of times in the search loop.

Use: Input a line for the pattern followed by a line for the text. The algorithm will print each
index in the text where the pattern appears or a blank line if the pattern does not appear.
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<int> find(string const& pattern, string const& text)
{
    // Calculate longest prefix suffix, in other words:
    // For each possible substring of the pattern, that starts at index 0 and ends at index i:
    // *Does the prefix match the suffix?
    // -> If yes store the length of the longest matching section in lps[i] 
    // -> If no store 0 in lps[i]
    vector<int> lps(pattern.size(), 0);
    for (int i{1}, j{}; i < pattern.size();)
    {
        if (pattern[i] == pattern[j])
        {
            j++;
            lps[i] = j;
            i++;
        }
        else
        {
            if (j != 0)
                j = lps[j-1]; // Check if we can match a smaller prefix/suffix than the failed one
            else
            {
                // Give up no prefix-suffix match
                lps[i] = 0;
                i += 1;
            }
        }
    }

    // The idea is that we can use lps to not start over from the beginning of the pattern every 
    // mismatch. We can instead start over from position lps[j-1], since we can guarantee that
    // pattern[0:lps[j-1]-1] still matches the text substring we were examining
    vector<int> result{};
    for (int i{}, j{}; i < text.size();)
    {
        if (pattern[j] == text[i])
        {
            i++;
            j++;
        }
        else if (i < text.size())
        {
            if (j != 0)
                j = lps[j-1]; // Check if we can match a smaller substring than the failed one
            else
                i++; // Give up -> no pattern can be found
        }

        if (j == pattern.size())
        {
            // Pattern found!
            result.push_back(i-pattern.size());
            // We know the text ends with a specific suffix
            // If that suffix also is a prefix in out pattern we continue on that
            j = lps[j-1]; 
        }
    }
    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string pattern{}, text{};

    while (getline(cin, pattern))
    {
        getline(cin, text);

        vector<int> matching_indices = find(pattern, text);

        for (int i : matching_indices)
        {
            cout << i << " ";
        }
        cout << "\n";
    }

    return 0;
}
