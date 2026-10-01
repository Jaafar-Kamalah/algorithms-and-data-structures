/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: addition, subtraciton, multiplication, division, comparison, input and outpu of 
rational numbers.

Implemented algorithm: Normalization of rational numbers so that there is only one exact
numerator and denominator combination for each unique value. Reduction of rational numbers
to preserve memory by division with gcd.

Time complexity: Arithmitic operations and construction runs in O(log(n)), where n is the the 
size of the largest between the numerator and the denominator. This is because they require
reduction with gcd wich runs in O(log(n)). Everything else runs in constant time.

Use: Input the number of operations to resolve (<INT_MAX), followed by 2 integers (<LONGLONG_MAX)
for the first rational number, a operator (+, -, / or *) and 2 more integers for the seconds
rational number. The resukting rational number will be printed with a slash charachter (ex: "5 / 6").
*/

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

using ll = long long;

class Rational
{
    public:
    Rational() : numerator{}, denominator{}
    {}

    Rational(ll const& n, ll const& d) : numerator{n}, denominator{d}
    {
        this->normalize();
        this->reduce();
    }

    void normalize()
    {
        // A rational number needs to be representedin one and only one certain way
        // Therefore we force all 0 to be 0/1
        if (numerator == 0)
            denominator = 1;

        // We also force only the numerator to be signed
        else if (denominator < 0)
        {
            // -1/-1 -> 1/1, 1/-1 -> -1/1, -1/1 -> -1/1
            numerator *= -1;
            denominator *= -1;
        }
    }

    void reduce()
    {
        // Find the greatest common denominator to reduce both values
        ll greatest_common_denominator{gcd(numerator, denominator)};

        numerator /= greatest_common_denominator;
        denominator /= greatest_common_denominator;
    }

    Rational operator+(Rational const& rhs) const
    {
        // a/b + c/d = (a*d + c*b) / (d*b)
        // We can safely multiply twp Rational values because it is guaranteed <10^9
        // 10^9 * 10^9 = 10^18 < LongLongMax
        // Even 10^18 + 10^18 < LongLongMax
        Rational result{};
        result.numerator = numerator*rhs.denominator + rhs.numerator*denominator;
        result.denominator = denominator * rhs.denominator;

        result.normalize();
        result.reduce();

        return result;
    }

    Rational operator-(Rational const& rhs) const
    {
        Rational neg_rhs{-rhs.numerator, rhs.denominator};
        return *this + neg_rhs;
    }

    Rational operator*(Rational const& rhs) const
    {
        Rational result{numerator * rhs.numerator, denominator * rhs.denominator};
        return result;
    }

    Rational operator/(Rational const& rhs) const
    {
        // a/b / b/c = a/b * c/b
        Rational inv_rhs{rhs.denominator, rhs.numerator};
        return *this * inv_rhs;
    }

    bool operator==(Rational const& rhs) const
    {
        return (numerator == rhs.numerator && denominator == rhs.denominator);
    }

    bool operator>(Rational const& rhs) const
    {
        // a/b > c/d <-> a*d > c*b
        return (numerator * rhs.denominator > rhs.numerator * denominator);
    }

    bool operator!=(Rational const& rhs) const
    {
        return (!(*this == rhs));
    }

    bool operator<(Rational const& rhs) const
    {
        return (!(*this > rhs) && (*this != rhs));
    }

    bool operator<=(Rational const& rhs) const
    {
        return ((*this < rhs) || (*this == rhs));
    }

    bool operator>=(Rational const& rhs) const
    {
        return ((*this > rhs) || (*this == rhs));
    }

    ll numerator;
    ll denominator;
};

ostream& operator<<(ostream& os, Rational const& r)
{
    os << r.numerator << " / " << r.denominator;
    return os;
}

istream& operator>>(istream& is, Rational & r)
{
    is >> r.numerator >> r.denominator;
    r.normalize();
    r.reduce();
    return is;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int no_of_operations{};

    cin >> no_of_operations;

    for (int i{}; i < no_of_operations; i++)
    {
        Rational lhs{}, rhs{};
        char op{};
        cin >> lhs >> op >> rhs;

        switch (op)
        {
            case '+':
                cout << lhs+rhs << "\n";
                break;
            case '-':
                cout << lhs-rhs << "\n";
                break;
            case '/':
                cout << lhs/rhs << "\n";
                break;
            case '*':
                cout << lhs*rhs << "\n";
                break;
        }
    }

    return 0;
}
