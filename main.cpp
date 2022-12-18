///////////////////////////////////////////////////////////////
// MaaS_General Challenge: Eliminate overlapped convex hulls
// Written by Furkan Cetin
// 18/12/2022
//
// This code follows Google C++ Style Guide
///////////////////////////////////////////////////////////////
// JSON Parser library by Neils Lohmann: 
// https://github.com/nlohmann/json
//
// Convex Hull using Graham Scan: 
// https://www.geeksforgeeks.org/convex-hull-using-graham-scan/
//
// Algorithm for checking if a polygon contains a point: 
// https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
// Area calculation by using Shoelace formula
// https://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/
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
	// Prepare a vector for keeping all Polygons
	vector<Polygon*> listPolygon;

	// Loads the json data in where the points data are loaded
	ifstream strAll("data/convex_hulls (1).json");
    json dataAll = json::parse(strAll);

	auto dataPolygons = dataAll.items().begin(); //Passing in JSON: "convex hull"

    for (const auto& item : dataPolygons.value().items())
    {
		auto dataPoints = item.value().items().begin(); //Passing in JSON: "ID"
		dataPoints++;									//Passing in JSON: "apexes"

		// Calculates the number of total points
		int sizePoints = 0;
		for (const auto& val : dataPoints.value().items()){
			sizePoints++;
		}

        if (sizePoints < 3)
            continue;

		// Combines all points into an array
		Point pointData[sizePoints];
		
		int i = 0;
		for (const auto& val : dataPoints.value().items())
        {
			pointData[i].x = val.value()["x"];
			pointData[i].y = val.value()["y"];

			i++;
        }

		// Creats a new Polygon object
		auto newPolygon = new Polygon();
		newPolygon->createFromConvexHull( pointData , i);
		listPolygon.push_back(newPolygon);
    }

	cout << endl << "There are " << listPolygon.size() << " polygons loaded" << endl;

    // Creates an array for keeping scores for collisions
    bool toBeRemovedLater[listPolygon.size()];
    for(int i=0;i< listPolygon.size(); i++)
        toBeRemovedLater[i] = false;

    // Checks colision of polygons with their bounding boxes (Bounding box collision is used for better performance)
    int i = 0, j = 1;
    for (auto it1 = listPolygon.begin(); it1 != listPolygon.end()-1; it1++)
    {
        for (auto it2 = it1+1; it2 != listPolygon.end(); it2++)
        {
            cout << "Checking " << i << " vs " << j << ":" << endl;  
            bool isBboxColliding = (*it1)->isCollidingWithPolygonFast(*it2);

            // CASE #0: No collision at all
            // Action: Continues scanning polygons
            if (isBboxColliding == false){
                j++;
                continue;
            } 
                

            // Finds the intersecting points of polygons
            std::vector<Point> collisionPoints = (*it1)->isCollidingWithPolygon(*it2);
            bool isLineColliding = collisionPoints.size() > 0;
            cout << "Collision detected: " << i << " vs " << j << " (" << isLineColliding << "|" << isBboxColliding  << ")"<< endl;

            // CASE #1: One polygon fully contains the other polygon (Polygons have no line collision but boundingbox)
            // Action: Removes the polygon with smaller area (which is %100 overlapped with other bigger polygon)
            if (isBboxColliding && !isLineColliding)
            {
                // IMPORTANT NOTE: There is exceptional cases where the smaller polygon would be completely outside of the bigger polygon
                // yet they could have colliding bounding boxes
                // There should be checking if at least one point is inside of the bigger polygon or not
                if ((*it1)->getArea() < (*it2)->getArea()) //When it1 is smaller
                {
                    if ((*it2)->isPointInside((*it1)->getPointAt(0))) //Checks if it1 has any point within it2 polygon
                    {
                        cout << "Polygon #" << i << " has %100 of its area overlapping"  << endl;
                        toBeRemovedLater[i] = true; //Marks the smaller polygon to be removed later
                    }
                }
                else
                {
                    if ((*it1)->isPointInside((*it2)->getPointAt(0))) //Checks if it2 has any point within it1 polygon
                    {
                        cout << "Polygon #" << j << " has %100 of its area overlapping"  << endl;
                        toBeRemovedLater[j] = true; //Marks the smaller polygon to be removed later
                    }
                }
                
                j++;
                continue;        
            }

            // CASE #2: Polygons have partly collision. A new Polygon is created out of intersection of Polygons.
            // Action: Removes the polygon which has %50 of its area overlapping with other polygon
            if (isBboxColliding && isLineColliding) 
            {
                std:vector<Point> pointsIntersection;

                // STEP #1: Includes all points of Polygon it1 that are covered by Polygon it2
                std::vector<Point> pointsOf1in2 = (*it1)->allCoveredPoints((*it2));
                for(Point pp : pointsOf1in2) 
                {
                    pointsIntersection.push_back(pp);
                }

                // STEP #2: Includes all points of Polygon it2 that are covered by Polygon it1
                std::vector<Point> pointsOf2in1 = (*it2)->allCoveredPoints((*it1));
                for(Point pp : pointsOf2in1) 
                {
                    pointsIntersection.push_back(pp);
                }

                // STEP #3: Includes all crossection points of Polygon it1 and Polygon it2
                for(Point pp : collisionPoints) 
                {
                    //cout << "csp: "<< pp.x << "," << pp.y << endl;
                    pointsIntersection.push_back(pp);
                }

                // STEP #4: Passes point data as an array to the Polygon object for convex hull algorithm
                Point arrayPoints[pointsIntersection.size()];
                //cout << "TOTAL: "<< pointsIntersection.size() << "=" << collisionPoints.size() << endl;

                int k = 0;
                for(Point pp : pointsIntersection) 
                {
                    arrayPoints[k] = pp;
                    k++;
                }

                // Finds the polygon at intersection
                Polygon polyIntersection;
                polyIntersection.createFromConvexHull(arrayPoints,k);

                // Gets all the area values of polygons
                float areaIntersection = polyIntersection.getArea();
                float areaPolygon_i = (*it1)->getArea();
                float areaPolygon_j = (*it2)->getArea();

                // Marks the polygon to be removed later if this area is at least %50 of the polygon 
                if ( (areaIntersection/areaPolygon_i) > 0.5f){
                    cout << "Polygon #" << i << " has %" << 100*(areaIntersection/areaPolygon_i) << " of its area overlapping"  << endl;
                    toBeRemovedLater[i] = true;
                }
                    

                if ( (areaIntersection/areaPolygon_j) > 0.5f){
                    cout << "Polygon #" << j << " has %" << 100*(areaIntersection/areaPolygon_j) << " of its area overlapping"  << endl;
                    toBeRemovedLater[j] = true;
                }
                    
            }
            j++;
        }
        i++;
        j = i+1;
    }

    cout << endl; 
    //Removes marked Polygons
    i=0;
    for (auto it = listPolygon.begin(); it != listPolygon.end(); )
    {
        if (toBeRemovedLater[i])
        {
            cout << "Deleting Polygon #" << i << endl;
            listPolygon.erase(it);
        }
        else
            it++;
        
        i++;
    }

    cout << endl << "There are " << listPolygon.size() << " polygons left" << endl;

    // Printing into a new json file
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

    std::ofstream o("data/result_convex_hulls.json");
    o << std::setw(3) << outputJson << std::endl;

    cout << endl << "Printed into a file: data/result_convex_hulls.json" << endl;

    o.close();
	return 0;
}