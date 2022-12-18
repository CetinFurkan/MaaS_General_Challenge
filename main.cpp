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
	//Keeping all Polygons an a vector
	vector<Polygon*> listPolygon;

	//Loading the json data in where the points data are loaded
	ifstream strAll("data/convex_hulls (1).json");
    json dataAll = json::parse(strAll);

	auto dataPolygons = dataAll.items().begin(); //Passing in JSON: "convex hull"

    for (const auto& item : dataPolygons.value().items())
    {
		auto dataPoints = item.value().items().begin(); //Passing in JSON: "ID"
		dataPoints++;									//Passing in JSON: "apexes"

		//Calculating the number of total points
		int sizePoints = 0;
		for (const auto& val : dataPoints.value().items()){
			sizePoints++;
		}

		//Combining all points into an array
		Point pointData[sizePoints];
		
		int i = 0;
		for (const auto& val : dataPoints.value().items())
        {
			pointData[i].x = val.value()["x"];
			pointData[i].y = val.value()["y"];

			i++;
        }

		//Creating a new Polygon object
		auto newPolygon = new Polygon();
		newPolygon->createFromConvexHull( pointData , i);
		listPolygon.push_back(newPolygon);
    }

	cout << "There are " << listPolygon.size() << " polygons loaded" << endl;

    // Creating a scores for keeping collision numbers and setting to zeros
    bool toBeRemovedLater[listPolygon.size()];
    for(int i=0;i< listPolygon.size(); i++)
        toBeRemovedLater[i] = false;

    // Checking colision of polygons with their bounding boxes
    // Bounding box collision is used for better performance
    for(int i=0;i< listPolygon.size()-1; i++)
    {
        for(int j=i+1;j< listPolygon.size(); j++)
        {
            cout << "Checking " << i << " vs " << j << ":" << endl;  
            bool collisionBbox = listPolygon.at(i)->isCollidingWithPolygonFast(listPolygon.at(j));
            cout << collisionBbox << endl;  
            // CASE #0: No collision at all
            // Action: Continue scanning polygons
            if (collisionBbox == false) 
                continue;

            bool collisionLines = listPolygon.at(i)->isCollidingWithPolygon(listPolygon.at(j));
            cout << " " << collisionLines << endl;    
            // CASE #1: One polygon fully contains the other polygon (Polygons have no line collision but boundingbox)
            // Action: Remove the polygon with smaller area (which is %100 overlapped with other bigger polygon)
            if (collisionBbox && !collisionLines)
            {
                if (listPolygon.at(i)->getArea() < listPolygon.at(j)->getArea())
                    toBeRemovedLater[i] = true;
                else
                    toBeRemovedLater[j] = true;

                continue;        
            }

            // CASE #2: Polygons have partly collision
            // Action: Remove the polygon with smaller area (which is %100 overlapped with other bigger polygon)
            if (collisionBbox && collisionLines) 
            {
                //TODO: Find intersecting polygon
                //TODO: Compare the area with polygons overall area
                //TODO: Mark the polygon to be removed later if this area is at least %50 of the polygon 
            }

        }
    }

    for(int i=0;i< listPolygon.size(); i++)
    {
        cout << i << ":" << toBeRemovedLater[i] << endl;
    }

	return 0;
}