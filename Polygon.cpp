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
    plist_.push_back(p);
}

Point Polygon::getPointAt(int i){
    return plist_.at(i);
}

int Polygon::pointsSize(){
    return plist_.size();
}

Polygon::~Polygon(){

}    
