///////////////////////////////////////////////////////////////
// MaaS_General Challenge: Eliminate overlapped convex hulls
// Written by Furkan Cetin
// 18/12/2022
//
///////////////////////////////////////////////////////////////
// JSON Parser library by Neils Lohmann: https://github.com/nlohmann/json
///////////////////////////////////////////////////////////////

//STL Libraries
#include <iostream>
#include <fstream>

//JSON Library
#include "nlohmann/json.hpp"

//Custom Libraries
#include "Polygon.h"

using json = nlohmann::json;
using namespace std;

int main()
{
    //Loading the json data in where the points data are loaded
	ifstream strAll("data/convex_hulls (1).json");
    json dataAll = json::parse(strAll);

    auto dataPolygons = dataAll.items().begin(); //Passing in JSON: "convex hull"

    for (const auto& item : dataPolygons.value().items())
    {
		auto dataPoints = item.value().items().begin(); //Passing in JSON: "ID"
		dataPoints++;									//Passing in JSON: "apexes"

		for (const auto& val : dataPoints.value().items())
        {
            cout << val.value()["x"] << "|" << val.value()["y"] << endl;
        }
    }

    //Testing Polygon object
    auto testObject = new Polygon();
    testObject->appendPoint(12.3,45.6);
    testObject->appendPoint(00.6,99.8);
    cout << testObject->pointsSize() << endl;
    cout << testObject->getPointAt(1).x << "," << testObject->getPointAt(1).y << endl;
        

    //cout << dataAll["convex hulls"] << endl;
    cout << "There are " << dataAll["convex hulls"].size() << " polygons" << endl;

}