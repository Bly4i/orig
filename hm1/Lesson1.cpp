#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;



class bad_length : public runtime_error {
public:
    bad_length() : runtime_error("����� ������ ����� ��������� �����!") {}
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


    cout << "������� ��������� �����: ";
    cin >> forbidden_length;
    cin.ignore();
    string input;


    while (true) {
        cout << "������� �����: ";
        getline(cin, input);

        try {
            int length = function(input, forbidden_length);
            cout << "����� ����� " << input << " ����� " << length << endl;
        }
        catch (const bad_length&) {
            cout << "�� ����� ����� ��������� �����! �� ��������" << endl;
            break;
        }
    }



    return 0;
}





