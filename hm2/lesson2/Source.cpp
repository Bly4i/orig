#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include "Triangle.h"

using namespace std;





int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
  

    try {
        Triangle t(3, 4, 5, 40, 70, 50);
    }
    catch (const GeometricFigureException& e) {
        cout << e.what() << endl;
    }

    try {
        Triangle invalidTriangle(1, 2, 3, 40, 50, 90);
    }
    catch (const GeometricFigureException& e) {
       cout << e.what() << endl;
    }

    




    return 0;
}





