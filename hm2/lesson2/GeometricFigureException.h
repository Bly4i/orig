#ifndef GEOMETRIC_FIGURE_EXCEPTION_H
#define GEOMETRIC_FIGURE_EXCEPTION_H

#include <string>
using namespace std;

class GeometricFigureException : public exception {
public:
    explicit GeometricFigureException(const string& message)
        : message_(message) {}

    virtual const char* what() const noexcept {
        return message_.c_str();
    }

private:
    string message_;
};

#endif // GEOMETRIC_FIGURE_EXCEPTION_H