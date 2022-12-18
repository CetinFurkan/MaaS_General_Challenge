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

//STL Libraries
#include <iostream>
#include <fstream>
#include <stack>

//JSON Library
#include "nlohmann/json.hpp"

//Custom Libraries
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

	return 0;
}