#pragma once

#include <stack>
#include "types.h"

#define F_MAX 9999.9f;
#define F_MIN -9999.9f;

using namespace std;

Point p0; //A variable needed for convexhull calculation

namespace ch{
// Finds next to top in a stack
Point nextToTop(stack<Point> &S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}

// Swaps two points
void swap(Point &p1, Point &p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

// Retruns square of distance between p1 and p2
int distSq(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y);
}

// Finds the orientation of ordered triplet (p, q, r).
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

// Retruns if a point is on the line or not
bool onLine(Line l1, Point p)
{
    if (p.x <= std::max(l1.p1.x, l1.p2.x)
        && p.x <= std::min(l1.p1.x, l1.p2.x)
        && (p.y <= std::max(l1.p1.y, l1.p2.y)
            && p.y <= std::min(l1.p1.y, l1.p2.y)))
        return true;
 
    return false;
}

// Returns the intersecting point of two lines
Point lineLineIntersection(Line l1, Line l2)
{
    // Stores the values for fast access and easy equations-to-code conversion
    float x1 = l1.p1.x, x2 = l1.p2.x, x3 = l2.p1.x, x4 = l2.p2.x;
    float y1 = l1.p1.y, y2 = l1.p2.y, y3 = l2.p1.y, y4 = l2.p2.y;
    
    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    
    // If determinant is zero, there is no intersection
    if (d == 0) 
        return Point{-9999,-9999};
    
    // Gets the x and y
    float pre = (x1*y2 - y1*x2);
    float post = (x3*y4 - y3*x4);
    float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
    float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;
    
    // Checks if the x and y coordinates are within both lines
    if ( x < min(x1, x2) || x > max(x1, x2) || x < min(x3, x4) || x > max(x3, x4) ) 
        return Point{-9999,-9999};

    if ( y < min(y1, y2) || y > max(y1, y2) || y < min(y3, y4) || y > max(y3, y4) ) 
        return Point{-9999,-9999};
    
    // Returns the point of intersection
    return Point{x,y};

}

// Returns if two lines intersect
bool isLinesIntersecting(Line l1, Line l2){
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

//Returns if a point is inside of a rectangle area
bool isInside(Point p, Rect r){
    if (p.x >= r.p1.x && p.x <= r.p2.x )
        if (p.y >= r.p1.y && p.y <= r.p2.y )
            return true;

    return false;
}

//Returns if a rectangle has a collision with another rectangle
bool isCollidingRects(Rect r1, Rect r2){

    // If one rectangle is on left side of other
    if (r1.p1.x > r2.p2.x || r2.p1.x > r1.p2.x)
        return false;
 
    // If one rectangle is above other
    if (r1.p1.y > r2.p2.y || r2.p1.y > r1.p2.y)
        return false;

    return true;
}

}//ending namespace "ch"
