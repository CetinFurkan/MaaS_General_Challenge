#ifndef _POLYGON_
#define _POLYGON_

#include <stdlib.h>
#include <vector>

struct Point
{
	float x, y;
};

class Polygon
{
public:
    Polygon();
    ~Polygon();

    void appendPoint(float x, float y);
    Point getPointAt(int i);
    int pointsSize();

    void createFromConvexHull(Point points[], int n);

private:
    std::vector<Point> plist;
    
};

#endif //_POLYGON_