/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Calculate prime numbers in a range 1 to n.

Implemented algorithm: Erathostenes sieve. This method first assumes that all numbers >=2 are prime,
then crosses out multiples of each prime starting from 2 onward. When crossing out it is safe to
start at the square of the current prime being handled, if that squared value is larger than the max
number of the sieve it is safe to quit as all unhandled number are prime.

Time complexity: Approximately O(n*log(n)), where n is the max value of the sieve. This is 
because the inner loop performs approximately n/1 + n/2 + n/3 + ... + n/sqrt(n) iterations. This 
can also be expressed as n (1 + 1/2 + 1/3 + ... + 1/sqrt(n)) where the section in the parentheses 
is known as the harmonic series and has the approximation O(log(sqrt(n))). So in total it is
O(n*log(sqrt(n))) = O(n*log(n)).

Use: Input the max integer (<=long long max) of the sieve long with the number of queries 
(<=long long max) followed by the quries. The program will output the number pf prime numbers
in the range followed by a 1 or 0 for each query based on if it is or is not a prime number.
*/

#include <iostream>
#include <vector>

using namespace std;

using ll = long long;

struct Sieve
{
    Sieve(long long const& m) : max{m}, no_of_primes{max-1}, is_prime(max+1, true)
    {
        is_prime[0] = false;
        is_prime[1] = false;

        for(int i{2}; i * i <= max; i++)
        {
            if (is_prime[i])
            {
                for (int j{i*i}; j <= max; j += i)
                {
                    if (is_prime[j])
                    {
                        is_prime[j] = false;
                        no_of_primes--;
                    }
                }
            }
        }
    }

    long long max;
    long long no_of_primes;
    vector<bool> is_prime;
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    long long sieve_max{}, no_of_querie{};

    cin >> sieve_max >> no_of_querie;

    Sieve s{sieve_max};
    cout << s.no_of_primes << "\n";

    for (long long i{}; i < no_of_querie; i++)
    {
        long long query{};
        cin >> query;
        cout << s.is_prime[query] << "\n";
    }

    return 0;
}