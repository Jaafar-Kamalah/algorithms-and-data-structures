/*
Author: Jaafar Kamalah (jaaka770)

Problem solved: Calculate if a point is inside, outside or on the border of a polygon.

Implemented algorithm: Raycasting algorithm. For each point if one were to cast a horizontal line
in either direction and count the number of intersections between the ray and the polygon edges,
a odd number of intersections means the point is inside the polygon while a even number of edges 
means the point is outside the polygon (see code for how this is checked). Moreover, for on-border 
detection we use collinearity- and boundchecks.

Time complexity: O(N), where N i the total number of point in the polygon. This is because we loop
all point in the algorithm and don't do much else.

Use: Input the total number of points (<INT_MAX) followed by a x- and y-coordinate (<INT_MAX) for 
each point. Input the total number of points to test (<INT_MAX) followed by a x- and y-coordinate 
(<INT_MAX) for each point. For each point to test the program will output "in", "out" or "on", if 
inside, outide or on the border of the polygon. Input 0 to end the algorithm.
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

    // Take points A, B and returns vector A->B
    Point toVector(Point const& B) const
    {
        return Point{B.x - x, B.y - y};
    }


    T x;
    T y;
};

// Returns -1 if on border, 0 if outide polygon and 1 if inside polygon
int inside_poly(Point<int> p, vector<Point<int>> const& polygon)
{
    int intersections{};

    for (size_t i{}; i < polygon.size(); i++)
    {
        int j = i + 1;
        if (j == polygon.size())
            j = 0;

        Point A{polygon[i]};
        Point B{polygon[(j)]};

        // Detect if point p is on line A->B
        // Check first for if p, A and B are colinear
        Point AB = A.toVector(B);
        Point Ap = A.toVector(p);
        long area = AB.cross(Ap);

        if (area == 0)
        {
            // Points are colinear
            // Check if p is also within bounds of A->B to confirm
            if (p.x <= max(A.x, B.x) && p.x >= min(A.x, B.x) &&
                p.y <= max(A.y, B.y) && p.y >= min(A.y, B.y))
                return -1;
        }

        // Inside/outside detection with raycasting
        // Cast a ray to the right and count intersections
        // Even amount = outside, odd amount = inside
        if (p.y <= max(A.y, B.y) && p.y > min(A.y, B.y))
        {
            // The ray is in the correct verical position to intersect with the line A->B
            // To calculate if it is in the coorect horisontal position we need to calculate X0
            // X0 is the x-coordinate on the edge A-B where y equals p.y
            double y_travel = double(p.y - A.y) / (B.y - A.y);
            double X0 = A.x + y_travel * (B.x - A.x);
            if (X0 > p.x)
                intersections++;
        }
    }
    return (intersections % 2 == 0) ? 0 : 1;
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

        int no_of_queries{};

        cin >> no_of_queries;
        for(int i{}; i < no_of_queries; i++)
        {
            Point<int> p{};
            cin >> p.x >> p.y;
            int result = inside_poly(p, polygon);
            
            if (result == -1)
                cout << "on\n";
            else if (result == 0)
                cout << "out\n";
            else if (result == 1)
                cout << "in\n";

        }

        cin >> no_of_points;
    }

    return 0;
}