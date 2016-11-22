//
// Created by Hindrik Stegenga on 22-11-16.
//

#include <iostream>
#include "ShapeTestDecorator.h"

using namespace std;

ShapeTestDecorator::ShapeTestDecorator(Shape* _shape) : ShapeDecorator(_shape) {

}

void ShapeTestDecorator::draw() const {

    cout << "test" << endl;

    ShapeDecorator::draw();
}