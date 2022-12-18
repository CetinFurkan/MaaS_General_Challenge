#include "Polygon.h"

#include <iostream>

#include "utility.hpp"

Polygon::Polygon()
{
    clearPoints();
}

Polygon::~Polygon()
{

}    

void Polygon::appendPoint(float x, float y)
{
    Point p;
    p.x = x;
    p.y = y;
    plist_.push_back(p);
    pointsize_++;

    bbox_.p1.x = std::min(bbox_.p1.x , x);
    bbox_.p1.y = std::min(bbox_.p1.y , y);
    bbox_.p2.x = std::max(bbox_.p2.x , x);
    bbox_.p2.y = std::max(bbox_.p2.y , y);
}

void Polygon::appendPoint(Point p)
{
    appendPoint(p.x,p.y);
}

Point Polygon::getPointAt(int i)
{
    return plist_.at(i);
}

int Polygon::pointsSize()
{
    return pointsize_;
}

void Polygon::clearPoints()
{
    plist_.clear();
    pointsize_ = 0;
    area_ = 0.0f;
    bbox_.p1.x = F_MAX;
    bbox_.p1.y = F_MAX;
    bbox_.p2.x = F_MIN;
    bbox_.p2.y = F_MIN;
}

float Polygon::getArea()
{
    return area_;
}

//Returns the area of the polygon
void Polygon::calculateArea()
{
    if (pointsSize()< 3)
        return;

    // Initialize area
    float area = 0.0;
 
    // Calculate value of shoelace formula
    int j = pointsSize() - 1;
    for (int i = 0; i < pointsSize(); i++)
    {
        area += (getPointAt(j).x + getPointAt(i).x) * (getPointAt(j).y - getPointAt(i).y);
        j = i;  // j is previous vertex to i
    }
 
    // Set absolute value
    area_ = abs(area / 2.0);
}

// Prints convex hull of a set of n points.
void Polygon::createFromConvexHull(Point points[], int n)
{
    if (n == 0)
        return;

    //PROCESSING CODE GENERATOR FOR DEBUGGING:
    /*
    cout << "stroke(0,0,0);" << endl;
    cout << "strokeWeight(5);" << endl;
    cout << "beginShape(POINTS);" << endl;
    for(int i =0;i<n;i++){
        cout << "vertex(" << points[i].x*10 << "," << points[i].y*10 << ");" << endl;
    }
    cout << "endShape(CLOSE);" << endl;
    */

	// Finds the bottommost point
	float ymin = points[0].y; 
	int min = 0;
	for (int i = 1; i < n; i++)
	{
		float y = points[i].y;

		// Picks the bottom-most or choose the left most point in case of tie
		if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
        {
			ymin = points[i].y;
            min = i;
        }
	}

	// Place the bottom-most point at first position
	ch::swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.
	// A point p1 comes before p2 in sorted output if p2 has larger polar angle (in counterclockwise direction) than p1
	p0 = points[0];
	qsort(&points[1], n-1, sizeof(Point), ch::compare);

	// If two or more points make same angle with p0, Remove all but the one that is farthest from p0.
	// Remember that, in above sorting, our criteria was to keep the farthest point at the end when more than one points have same angle.
	int m = 1; // Initialize size of modified array
	for (int i=1; i<n; i++)
	{
		// Keeps removing i while angle of i and i+1 is same with respect to p0
		while (i < n-1 && ch::orientation(p0, points[i], points[i+1]) == 0)
			i++;

		points[m] = points[i];
		m++; // Update size of modified array
	}

	// If modified array of points has less than 3 points, convex hull is not possible
	if (m < 3) return;

	// Creates an empty stack and push first three points to it.
	stack<Point> S;
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by points next-to-top, top, and points[i] makes a non-left turn
		while (S.size()>1 && ch::orientation(ch::nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
        
		S.push(points[i]);
	}

	// Now stack has the boundry points of the polygon. They needed to be kept into plist vector
	clearPoints();
    //std::cout << "Polygon made of convex hull of the set: " << std::endl;


	while (!S.empty())
	{
		Point p = S.top();

        //std::cout << "(" << p.x << ", " << p.y <<")" << std::endl;
        appendPoint(p.x,p.y);

		S.pop();
	}

    //PROCESSING CODE GENERATOR FOR DEBUGGING:
    /*
    cout << "stroke(random(255),random(255),random(255));" << endl;
    cout << "strokeWeight(1);" << endl;
    cout << "beginShape();" << endl;
    for(int i =0;i<pointsSize();i++){
        cout << "vertex(" << getPointAt(i).x*10 << "," << getPointAt(i).y*10 << ");" << endl;
    }
    cout << "endShape(CLOSE);" << endl;
    */

    calculateArea();
    //std::cout << "Polygon area: " << area_ << std::endl;
}

// Returns if a point is included by polygon
bool Polygon::isPointInside(Point p)
{
    int counter = 0;
    int i;
    float xinters;
    Point p1,p2;

    p1 = getPointAt(0);

    for (i=1;i<=pointsize_;i++) 
    {
        p2 = getPointAt(i % pointsize_);
        
        if (p.y > std::min(p1.y,p2.y)) 
        {
            if (p.y <= std::max(p1.y,p2.y)) 
            {
                if (p.x <= std::max(p1.x,p2.x)) 
                {
                    if (p1.y != p2.y) 
                    {
                        xinters = (p.y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                        if (p1.x == p2.x || p.x <= xinters)
                            counter++;
                    }
                }
            }
        }
        p1 = p2;
    }

    if (counter % 2 == 0)
        return false;
    else
        return true;
}

// Returns collision points if polygon boundry has a collision with other polygon's boundry
// (std::vector is used because the output may have 0 point, 2 points or 4 points depending on the case)
std::vector<Point> Polygon::isCollidingWithPolygon(Polygon* other)
{
    // Initializes the vector for points
    std::vector<Point> ret;

    if (!other)
        return ret;

    // If polygons are not valid, returns
    if (pointsSize() < 3|| other->pointsSize() < 3)
        return ret;
    
    // Checks collision of each line of polygon with other polygon's lines
    for (int i=0;i<pointsSize();i++)
    {
        Line myLine;
        myLine.p1 = getPointAt(i);

        // Needed for considering the line that contains last point and first point
        if (i == pointsSize()-1) 
            myLine.p2 = getPointAt(0);
        else
            myLine.p2 = getPointAt(i+1);
        

        // Scans all lines of other polygon
        for (int j=0;j<other->pointsSize();j++){
            Line otherLine;
            otherLine.p1 = other->getPointAt(j);

            // Needed for considering the line that contains last point and first point
            if (j == other->pointsSize()-1) 
                otherLine.p2 = other->getPointAt(0);
            else
                otherLine.p2 = other->getPointAt(j+1);

            Point pp = ch::lineLineIntersection(myLine,otherLine); // Retruns {-9999,-9999} when no collision found
            if (pp.x != -9999) 
            {
                ret.push_back(pp);
                //cout << i << " x " << j << " Point:" << pp.x << "," << pp.y << endl;
            }
        }       
    }

    return ret;
}

// Returns if polygon's bounding box has collision with other polygon's bounding box
bool Polygon::isCollidingWithPolygonFast(Polygon* other)
{
    if (!other)
        return false;

    // If polygons are not valid, returns
    if (area_ == 0.0f || other->area_ == 0.0f)
        return false;
    
    if (ch::isCollidingRects(bbox_,other->bbox_))
        return true;
    
    return false;
}

//Returns list of points that are covered by another Polygon
std::vector<Point> Polygon::allCoveredPoints(Polygon* other){

    std::vector<Point> ret;

    if (!other)
        return ret;

    if (pointsSize() < 3 || other->pointsSize() < 3)
        return ret;
    
    for (auto it = plist_.begin(); it != plist_.end(); it++){
        if (other->isPointInside(*it))
            ret.push_back(*it);
    }

    return ret;
}

