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

    void createFromConvexHull(Point points[], int n);
    bool isPointInside(Point p);
    
private:
    std::vector<Point> plist_;
    float area_;

    void calculateArea();
    
};

#endif //_POLYGON_