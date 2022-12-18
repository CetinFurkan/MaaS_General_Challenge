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

private:
    std::vector<Point> plist; //List of points of polygon boundry!
};

#endif //_POLYGON_