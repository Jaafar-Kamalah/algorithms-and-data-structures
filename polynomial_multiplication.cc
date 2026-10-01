/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Multiplying two polynomials in O(n*logn) time complexity. This can be applied to two integers
of very large size aswell.

Implemented algorithm: Uses the fast fourier algorithm (see functions for more details). More specifcally 
prepares the two polynomials by adding padding and converting to complex vectors, calculates the fft for
each polynomial using divide and conquer, multiplies the two complex vectors element for element, calculating
the inverse fft for the product and converting the result back to real values.

Time complexity: O(nlogn), where n is the length of the input array given to fft() (this includes the padding).
Multiplying the fft values is O(n) since we do it element by element.

Space complexity: O(n)

Use: Input the number of multiplications, followed by the degree of the first polynomial and all the coeffecients
of that polynomial. Do the same for the second polynomial. If we want to calculate the product of two very large
values we can input the numbers from least significant digit to most significant and then add the resulting numbers
where we multiply each number with 10^i, i= 0, 1, ...
*/

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

// For any polynomial f(x), calculate the roots w of x^n = 1 , where n is the polynomial length.
// Then for each root in w calculate f(w). This is done using divide and conquer and butterfly pattern.
// For the inverse do the opposite.
void fft(vector<complex<double>> & a, bool inverse)
{
    // Base case
    if (a.size() == 1)
    {
        // fft(x)=x where x is a constant
        return;
    }

    // Split a into two vectors based on even and odd position
    vector<complex<double>> even{};
    vector<complex<double>> odd{};
    even.reserve(a.size() / 2);
    odd.reserve(a.size() / 2);
    for (unsigned long i{}; i < a.size(); i++)
    {
        if (i % 2 == 0)
        {
            even.push_back(a[i]);
        }
        else
        {
            odd.push_back(a[i]);
        }
    }

    fft(even, inverse);
    fft(odd, inverse);
    
    // Merge the even and odd numbers using this:
    // a[k] = even[k] + w*odd[0], k = [0,n/2-1]
    // a[k+n/2] = even[k] - w*odd[k] , k=[0,n/2-1]
    // Where w is root of unity for n

    // For the inverse
    double ang{2 * M_PI / a.size()};
    if (inverse)
    {
        ang *= -1;
    }
    complex<double> root_of_unity{1}, root_of_unity_step{cos(ang), sin(ang)};
    for (unsigned long i{}; 2*i < a.size(); i++)
    {
        a[i] = even[i] + root_of_unity * odd[i];
        a[i + a.size()/2] = even[i] - root_of_unity * odd[i];

        // For inverse we need to divide to undo the scaling
        if (inverse)
        {
            a[i] /= 2;
            a[i + a.size()/2] /= 2;
        }
        root_of_unity *= root_of_unity_step;
    }
}

vector<long> fast_multiply(vector<long> const& a, vector<long> const& b)
{
    if(a.empty() || b.empty())
    {
        return {};
    }
    vector<complex<double>> ac {a.begin(), a.end()};
    vector<complex<double>> bc {b.begin(), b.end()};

    // We need to pad each polynomial with enough zeroes to fit the product 
    long unsigned lhs_degree{a.size()-1}, b_degree{b.size()-1};
    long unsigned product_degree{lhs_degree + b_degree};
    long unsigned padding{product_degree + 1};

    // We also need to pad each polynomial so that the size is in the power of 2
    // x & (x-1) == 0 is only true when x is in power of 2: (1 & 0), (10 & 01) etc.
    while ((padding & (padding-1)) != 0)
    {
        padding++;
    }

    ac.resize(padding);
    bc.resize(padding);

    // Fast Fourier Transform both polynomials 
    fft(ac, false);
    fft(bc, false);

    // Multiply element for element and get the inverse
    vector<complex<double>> ac_bc_product(ac.size());
    for (unsigned long i{}; i < ac.size(); i++)
    {
        ac_bc_product[i] = ac[i] * bc[i];
    }
    fft(ac_bc_product, true);

    // Convert back to real values
    vector<long> result(product_degree + 1);
    for (unsigned long i{}; i < (product_degree +1); i++)
    {
        result[i] = round(ac_bc_product[i].real());
    }
    return result;
}

vector<long> get_polynomial()
{

    int lhs_degree;
    cin >> lhs_degree;
    vector<long> result{};
    long coef;
    for (int i{}; i <= lhs_degree; i++)
    {
        cin >> coef;
        result.push_back(coef);
    }
    return result;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int cases;
    cin >> cases;
    for (int i{}; i < cases; i++)
    {
        vector<long> a{get_polynomial()};
        vector<long> b{get_polynomial()};
        vector<long> product{fast_multiply(a, b)};
        cout << product.size() - 1 << "\n";

        for(long const& coef : product)
        {
            cout << coef << " ";
        }
        cout << "\n";
    }
    return 0;
}