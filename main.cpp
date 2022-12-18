///////////////////////////////////////////////////////////////
// MaaS_General Challenge: Eliminate overlapped convex hulls
// Written by Furkan Cetin
// 18/12/2022
//
// This code follows Google C++ Style Guide.
//
///////////////////////////////////////////////////////////////
// JSON Parser library by Neils Lohmann: https://github.com/nlohmann/json
///////////////////////////////////////////////////////////////
// Convex Hull using Graham Scan: https://www.geeksforgeeks.org/convex-hull-using-graham-scan/
///////////////////////////////////////////////////////////////
// Algorithm for checking if a polygon contains a point: https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
///////////////////////////////////////////////////////////////

//STL Libraries
#include <iostream>
#include <fstream>
#include <stack>

//JSON Library
#include "nlohmann/json.hpp"

//Custom Libraries
#include "types.h"
#include "Polygon.h"

using json = nlohmann::json;
using namespace std;

int main()
{
	//Keeps all polygons in a vector
	vector<Polygon*> listPolygon;

	//Loads the json data in where the points data are loaded
	ifstream strAll("data/convex_hulls (1).json");
    json dataAll = json::parse(strAll);

	auto dataPolygons = dataAll.items().begin(); //Passing in JSON: "convex hull"

    for (const auto& item : dataPolygons.value().items())
    {
		auto dataPoints = item.value().items().begin(); //Passing in JSON: "ID"
		dataPoints++;									//Passing in JSON: "apexes"

		//Calculates the number of total points
		int sizePoints = 0;
		for (const auto& val : dataPoints.value().items()){
			sizePoints++;
		}

		//Combines all points into an array
		Point pointData[sizePoints];
		
		int i = 0;
		for (const auto& val : dataPoints.value().items())
        {
			pointData[i].x = val.value()["x"];
			pointData[i].y = val.value()["y"];

			i++;
        }

		//Creats a new Polygon object
		auto newPolygon = new Polygon();
		newPolygon->createFromConvexHull( pointData , i);
		listPolygon.push_back(newPolygon);
    }

	cout << "There are " << listPolygon.size() << " polygons loaded" << endl;

    // Creates a scores for keeping collision numbers and setting to zeros
    bool toBeRemovedLater[listPolygon.size()];
    for(int i=0;i< listPolygon.size(); i++)
        toBeRemovedLater[i] = false;

    // Checks colision of polygons with their bounding boxes (Bounding box collision is used for better performance)
    for(int i=0;i< listPolygon.size()-1; i++)
    {
        for(int j=i+1;j< listPolygon.size(); j++)
        {
            //cout << "Checking " << i << " vs " << j << ":" << endl;  
            bool isBboxColliding = listPolygon.at(i)->isCollidingWithPolygonFast(listPolygon.at(j));

            // CASE #0: No collision at all
            // Action: Continues scanning polygons
            if (isBboxColliding == false) 
                continue;

            // Finds the intersecting points of polygons
            std::vector<Point> collisionPoints = listPolygon.at(i)->isCollidingWithPolygon(listPolygon.at(j));
            bool isLineColliding = collisionPoints.size() > 0;
            //cout << i << " vs " << j << ": " << isLineColliding << "|" << isBboxColliding  << endl;

            // CASE #1: One polygon fully contains the other polygon (Polygons have no line collision but boundingbox)
            // Action: Removes the polygon with smaller area (which is %100 overlapped with other bigger polygon)
            if (isBboxColliding && !isLineColliding)
            {
                int polySmaller = j;
                int polyBigger = i;
                if (listPolygon.at(i)->getArea() < listPolygon.at(j)->getArea())
                {
                    polySmaller = i;
                    polyBigger = j;
                }

                // IMPORTANT NOTE: There is exceptional cases where the smaller polygon would be completely outside of the bigger polygon
                // yet they could have colliding bounding boxes
                // There should be checking if at least one point is inside of the bigger polygon or not
                if (listPolygon.at(polyBigger)->isPointInside(listPolygon.at(polySmaller)->getPointAt(0)))
                {
                    toBeRemovedLater[polySmaller] = true; //Mark the smaller polygon to be removed later
                }
                    
                continue;        
            }

            // CASE #2: Polygons have partly collision
            // Action: Removes the polygon which has %50 of its area overlapping with other polygon
            if (isBboxColliding && isLineColliding) 
            {
                std:vector<Point> pointsCrossection;

                // STEP #1: Includes all points of Polygon #i that are covered by Polygon #j
                for( int k=0; k < listPolygon.at(i)->pointsSize(); k++)
                {
                    if (listPolygon.at(j)->isPointInside(listPolygon.at(i)->getPointAt(k)))
                        pointsCrossection.push_back(listPolygon.at(i)->getPointAt(k)); 
                }

                // STEP #2: Includes all points of Polygon #j that are covered by Polygon #i
                for( int k=0; k < listPolygon.at(j)->pointsSize(); k++)
                {
                    if (listPolygon.at(i)->isPointInside(listPolygon.at(j)->getPointAt(k)))
                        pointsCrossection.push_back(listPolygon.at(j)->getPointAt(k)); 
                }

                // STEP #3: Includes all crossection points of Polygon #j and Polygon #i
                for(Point pp : collisionPoints) 
                {
                    //cout << "csp: "<< pp.x << "," << pp.y << endl;
                    pointsCrossection.push_back(pp);
                }

                // STEP #4: Passes point data as an array to the Polygon object for convex hull algorithm
                Point arrayPoints[pointsCrossection.size()];
                //cout << "TOTAL: "<< pointsCrossection.size() << "=" << collisionPoints.size() << endl;

                int k = 0;
                for(Point pp : pointsCrossection) 
                {
                    arrayPoints[k] = pp;
                    k++;
                }

                // Finds intersecting polygon
                Polygon polyCrossection;
                polyCrossection.createFromConvexHull(arrayPoints,k);

                // Gets all the area values of polygons
                float areaCrossection = polyCrossection.getArea();
                float areaPolygon_i = listPolygon.at(i)->getArea();
                float areaPolygon_j = listPolygon.at(j)->getArea();

                // Marks the polygon to be removed later if this area is at least %50 of the polygon 
                if ( (areaCrossection/areaPolygon_i) > 0.5f)
                    toBeRemovedLater[i] = true;

                if ( (areaCrossection/areaPolygon_j) > 0.5f)
                    toBeRemovedLater[j] = true;
            }
        }
    }

    //Removes marked Polygons
    int i=0;
    for (auto it = listPolygon.begin(); it != listPolygon.end(); )
    {
        if (toBeRemovedLater[i])
        {
            listPolygon.erase(it);
        }
        else
            it++;
        
        i++;
    }

    cout << "There are " << listPolygon.size() << " polygons left" << endl;


    //Printing into new json file

    json outputJson;

    
    i = 0;
    for (auto it = listPolygon.begin(); it != listPolygon.end(); it++)
    {
        outputJson["convex hulls"][i]["ID"] = i;

        for (int j = 0; j < (*it)->pointsSize(); j++){
            outputJson["convex hulls"][i]["apexes"][j]["x"] = (*it)->getPointAt(j).x;
            outputJson["convex hulls"][i]["apexes"][j]["y"] = (*it)->getPointAt(j).y;
        }

        i++;
    }

    std::ofstream o("data/convex_hull_uneliminated.json");
    o << std::setw(3) << outputJson << std::endl;

	return 0;
}