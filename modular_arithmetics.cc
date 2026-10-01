/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Modular arithmetic operations including addition, subtraction multiplication and 
division (modular inverse);

Implemented algorithm: Recursive halving and extended eucledian algorithm. Recursive halving was
used for multiplication since the size of mod_mult arguments could cause a overflow in long long.
Morover, the extended euclidean algorithm which computes gcd of val and mod answell as finding
the integers x and y where val*x + mod*y = gcd(val,mod) in a recursive manner.

Time complexity: mod_add and mod_sub are resolved in constant time. mod_mult has runs in O(log(rhs)),
since rhs is halved in each call until it reaches 1. The extended euclidean algorithm runs in around
O(log(min(a,b))) because each recursive step we replace (val, mod) with (mod, val % mod), where
val % mod on drops exponentially.

Use: Input the mod value (<= LONGLONG_MAX), the number operations that will be called (<= INT_MAX),
followed by a integer (<= LONGLONG_MAX), operation (+, -, * or /) and an integer (<= LONGLONG_MAX)
for each operation. Input 0 0 to end the program. The output will produce the result of the modular
operation or -1 if no solution exists. The mod value needs to be larger than zero.
*/

#include <iostream>

using namespace std;

using ll = long long;

ll mod_add(ll const& lhs, ll const& rhs, ll const& mod)
{
    // The % operator in C++ does not work like the mathematical modulo on negative values
    // -5 % 100 == -5 in c++
    // -5 % 100 == 95 in math
    // We fix this by adding mod to the result if it is negative
    ll result {(lhs + rhs) % mod};
    if (result < 0)
        result += mod;
    return result;
}

ll mod_sub(ll const& lhs, ll const& rhs, ll const& mod)
{
    ll result {(lhs - rhs) % mod};
    if (result < 0)
        result += mod;
    return result;
}

ll safe_mult(ll const& lhs, ll const& rhs, ll const& mod)
{
    // Multiplication of two values as large as 10^18 would overflow a long long
    // Therefore we use a recursive algorithm, calculating the product without multiplication

    if (rhs == 0)
        return 0;
    
    if (rhs == 1)
        return lhs % mod; // Works because (a * b) % m = ((a % m) * (b % m)) % m
    
    // We divide one of the terms with 2 and then use addition to compensate the division
    ll half_product = safe_mult(lhs, rhs/2, mod);
    ll result = (half_product + half_product) % mod;

    // Dividing the rhs with 2 and compsensating with addition will decrease the result with 1*lhs
    if (rhs % 2 == 1)
        result = (result + lhs) % mod; // Works because (a + b) % m <-> ((a % m) + (b % m)) % m

    return  result;
}

ll mod_mult(ll  lhs, ll  rhs, ll const& mod)
{
    // We take do a mathematical mod on both factors first, so safe mod gets two positive arguments
    // Works because (a * b) % m = ((a % m) * (b % m)) % m
    lhs %= mod;
    if (lhs < 0)
        lhs += mod;

    rhs %= mod;
    if (rhs < 0)
        rhs += mod;

    ll result {safe_mult(lhs, rhs, mod)};
    return result;
}

tuple<ll, ll, ll> extended_euclidean(ll val, ll mod)
{
    // Calculates gcd, a and b where a*val + mod*b = gcd and gcd is the greates common denominator
    if (mod == 0)
        return {val, 1, 0};

    tuple result = extended_euclidean(mod, val % mod);
    return {get<0>(result), get<2>(result), (get<1>(result) - (val/mod) * get<2>(result))};
}

ll mod_inverse(ll val, ll mod)
{
    tuple<ll, ll, ll> result = extended_euclidean(val, mod);
    ll gcd = get<0>(result); // Greatest common denominator between val and mod
    ll a = get<1>(result); // val*a + mod*b = gcd


    if (gcd != 1)
        return -1; // Inverse does not exist

    a %= mod;
    if (a < 0)
        a += mod;

    return a;
}

ll mod_div(ll  lhs, ll  rhs, ll const& mod)
{
    // x / y is defined to be x * y^-1
    ll inv = mod_inverse(rhs, mod);

    if (inv == -1)
        return -1; // The modular inverse does not exist

    return mod_mult(lhs, inv, mod);
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll mod{};
    int no_of_operations{};

    cin >> mod >> no_of_operations;

    while (mod != 0)
    {
        for (int i{}; i < no_of_operations; i++)
        {
            ll lhs{}, rhs{}, result{};
            char op{};
            cin >> lhs >> op >> rhs;

            switch (op)
            {
                case '+':
                    result = mod_add(lhs, rhs, mod);
                    break;
                case '-':
                    result = mod_sub(lhs, rhs, mod);
                    break;
                case '*':
                    result = mod_mult(lhs, rhs, mod);
                    break;
                case '/':
                    result = mod_div(lhs, rhs, mod);
                    break;
            }
            cout << result << '\n';
        }
        cin >> mod >> no_of_operations;
    }

    return 0;
}
