#pragma once

#include <stack>
#include "types.h"

using namespace std;

Point p0; //A variable needed for convexhull calculation

namespace ch{
// A utility function to find next to top in a stack
Point nextToTop(stack<Point> &S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}

// A utility function to swap two points
void swap(Point &p1, Point &p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
			(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0; // collinear
	return (val > 0)? 1: 2; // clock or counterclock wise
}


// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;

    return (o == 2)? -1: 1;
}

// A utility function to check if a point is on the line or not
bool onLine(Line l1, Point p)
{
    if (p.x <= std::max(l1.p1.x, l1.p2.x)
        && p.x <= std::min(l1.p1.x, l1.p2.x)
        && (p.y <= std::max(l1.p1.y, l1.p2.y)
            && p.y <= std::min(l1.p1.y, l1.p2.y)))
        return true;
 
    return false;
}

// A utility function to check if two line intersects or not
bool isIntersect(Line l1, Line l2)
{
    // Four direction for two lines and points of other line
    int dir1 = orientation(l1.p1, l1.p2, l2.p1);
    int dir2 = orientation(l1.p1, l1.p2, l2.p2);
    int dir3 = orientation(l2.p1, l2.p2, l1.p1);
    int dir4 = orientation(l2.p1, l2.p2, l1.p2);
 
    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;
 
    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1))
        return true;
 
    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2))
        return true;
 
    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1))
        return true;
 
    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2))
        return true;
 
    return false;
}


}//ending namespace
