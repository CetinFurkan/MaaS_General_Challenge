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

    void appendPoint(float x, float y);
    Point getPointAt(int i);
    int pointsSize();
    void clearPoints();
    float getArea();

    void createFromConvexHull(Point points[], int n);
    bool isPointInside(Point p);
    bool isCollidingWithPolygon(Polygon* other); //Using actual polygon boundries
    bool isCollidingWithPolygonFast(Polygon* other); //Using bounding boxes
    
private:
    std::vector<Point> plist_;
    float area_ = 0.0f;
    Rect bbox_;

    void calculateArea();
    
};

#endif //_POLYGON_