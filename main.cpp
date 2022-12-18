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

using json = nlohmann::json;
using namespace std;

int main()
{
    //Loading the json data in where the points data are loaded
	ifstream strAll("data/convex_hulls (1).json");
    json dataAll = json::parse(strAll);

    cout << dataAll["convex hulls"] << endl;
    cout << "There are " << dataAll["convex hulls"].size() << " polygons" << endl;

}