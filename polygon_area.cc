/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Calculate the area of a polygon and the rotation that the points of the polygon were 
given in.

Implemented algorithm: The shoelace method. The name comes from that if yo verticallyu print the x- 
and y-coordinate of the points in either clockwise or counterclockwise, you can draw croses between 
the values (like a shoelace) and multiply. You can then sum up the products of each shoelace, 
subtract the two sums, take the absolute value and divide by 2 to get the total area. Note that the 
last of the printed coordinates is a duplicate of the first printed coordinate. Before taking the 
absokute value you can determine if the points where given in clockwise or counterclockwise order 
based on the sign of the value. 

Time complexity: O(N), where N i the total number of point in the polygon. This is because we loop
all point in the algorithm and don't do much else.

Use: Input the total number of points (<INT_MAX) followed by a x- and y-coordinate (<INT_MAX) for 
each point. Input 0 to end the algorithm. The output consists of CW for clockwise or CCW for
counterclockwise and the total area with one digit precision.
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

template <typename T>
struct Point
{
    // Constructors
    Point(T const& a, T const& b) : x(a), y(b)
    {}

    Point() : x{}, y{}
    {}

    // Addition & subtraction
    Point operator+(Point const& rhs) const
    {
        return Point{x + rhs.x, y + rhs.y};
    }

    Point operator-(Point const& rhs) const
    {
        return Point{x - rhs.x, y - rhs.y};
    }

    // Scalar multiplication & division
    Point operator*(T const& val) const
    {
        return Point{x * val, y * val};
    }

    Point operator/(T const& val) const
    {
        return Point{x / val, y / val};
    }

    // Dot product
    T dot(Point const& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    // Cross product
    T cross(Point const& rhs) const
    {
        return x * rhs.y - y * rhs.x;
    }

    T x;
    T y;
};

double polygon_area(vector<Point<int>> const& polygon)
{
    double right_lace{};
    double left_lace{};
    for (size_t i{1}; i < polygon.size(); i++)
    {
        right_lace += polygon[i-1].x * polygon[i].y;
        left_lace += polygon[i-1].y * polygon[i].x;
    }
    right_lace += polygon[polygon.size()-1].x * polygon[0].y;
    left_lace += polygon[polygon.size()-1].y * polygon[0].x;

    return (right_lace - left_lace) * 0.5;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int no_of_points{};
    cin >> no_of_points;

    while (no_of_points > 0)
    {
        vector<Point<int>> polygon{};
        for (int i{}; i < no_of_points; i++)
        {
            Point<int> p{};
            cin >> p.x >> p.y;
            polygon.push_back(p);
        }

        double signed_area = polygon_area(polygon);
        string rotation{};

        if (signed_area > 0)
            rotation = "CCW";
        else
            rotation = "CW";
        
        cout << rotation << " " << fixed << setprecision(1) <<  fabs(signed_area) << "\n";

        cin >> no_of_points;
    }

    return 0;
}