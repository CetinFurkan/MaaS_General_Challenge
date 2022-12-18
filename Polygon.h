#ifndef _POLYGON_
#define _POLYGON_

#include <stdlib.h>
#include <vector>

#include "types.h"

class Polygon
{
public:
    Polygon();
    ~Polygon();

    Point getPointAt(int i);
    int pointsSize();
    void clearPoints();
    float getArea();

    void createFromConvexHull(Point points[], int n);
    bool isPointInside(Point p);
    std::vector<Point> isCollidingWithPolygon(Polygon* other); //Using actual polygon boundries
    bool isCollidingWithPolygonFast(Polygon* other); //Using bounding boxes
    std::vector<Point> allCoveredPoints(Polygon* other);

private:
    std::vector<Point> plist_;
    float area_ = 0.0f;
    Rect bbox_;

    void calculateArea();
    void appendPoint(float x, float y);
    void appendPoint(Point p);
};

#endif //_POLYGON_