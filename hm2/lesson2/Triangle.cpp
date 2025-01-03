
#include "Triangle.h"

Triangle::Triangle(double a, double b, double c, double angleA, double angleB, double angleC)
    : sideA(a), sideB(b), sideC(c), angleA(angleA), angleB(angleB), angleC(angleC) {

    if (a <= 0 || b <= 0 || c <= 0) {
        throw GeometricFigureException("������ �������� ������. �������: ������� ������ ���� ��������������.");
    }
    if (angleA + angleB + angleC != 180) {
        throw GeometricFigureException("������ �������� ������. �������: ����� ����� �� ����� 180.");
    }

   cout << "����������� (������� " << sideA << ", " << sideB << ", " << sideC
        << "; ���� " << angleA << ", " << angleB << ", " << angleC << ") ������." << endl;
}