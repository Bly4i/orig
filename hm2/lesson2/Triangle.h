#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "GeometricFigureException.h"

class Triangle {
public:
    Triangle(double a, double b, double c, double angleA, double angleB, double angleC);

private:
    double sideA, sideB, sideC;
    double angleA, angleB, angleC;
};

#endif // TRIANGLE_H
