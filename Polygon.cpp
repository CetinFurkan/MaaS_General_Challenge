#include "Polygon.h"

#include <iostream>

using namespace std;

Polygon::Polygon(){
    cout << "Polygon object is created." << endl;
}

void Polygon::appendPoint(float x, float y){
    Point p;
    p.x = x;
    p.y = y;
    plist.push_back(p);
}

Point Polygon::getPointAt(int i){
    return plist.at(i);
}

int Polygon::pointsSize(){
    return plist.size();
}

Polygon::~Polygon(){

}    
