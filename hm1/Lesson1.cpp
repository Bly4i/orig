#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;



class bad_length : public runtime_error {
public:
    bad_length() : runtime_error("Длина строки равна запретной длине!") {}
};

int function(const ::string& str, int forbidden_length) {
    int length = str.size();
    if (length == forbidden_length) {
        throw bad_length();
    }
    return length;
}

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    int forbidden_length;


    cout << "Введите запретную длину: ";
    cin >> forbidden_length;
    cin.ignore();
    string input;


    while (true) {
        cout << "Введите слово: ";
        getline(cin, input);

        try {
            int length = function(input, forbidden_length);
            cout << "Длина слова " << input << " равна " << length << endl;
        }
        catch (const bad_length&) {
            cout << "Вы ввели слово запретной длины! До свидания" << endl;
            break;
        }
    }



    return 0;
}





