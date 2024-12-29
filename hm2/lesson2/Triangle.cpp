
#include "Triangle.h"

Triangle::Triangle(double a, double b, double c, double angleA, double angleB, double angleC)
    : sideA(a), sideB(b), sideC(c), angleA(angleA), angleB(angleB), angleC(angleC) {

    if (a <= 0 || b <= 0 || c <= 0) {
        throw GeometricFigureException("Ошибка создания фигуры. Причина: стороны должны быть положительными.");
    }
    if (angleA + angleB + angleC != 180) {
        throw GeometricFigureException("Ошибка создания фигуры. Причина: сумма углов не равна 180.");
    }

   cout << "Треугольник (стороны " << sideA << ", " << sideB << ", " << sideC
        << "; углы " << angleA << ", " << angleB << ", " << angleC << ") создан." << endl;
}