#ifndef CIRCLE_H
#define CIRCLE_H

#include "figure.h"
#include <cmath>


class Circle : public Figure {
private:
    double radius;

public:

    Circle();
    

    explicit Circle(double r);
    

    Circle(const Circle& other);
    

    Circle& operator=(const Circle& other);
    

    virtual ~Circle();
    

    double getRadius() const;
    

    double getArea() const override;
    

    double getPerimeter() const override;
    

    void printInfo() const override;
    

    void saveToFile(std::ostream& out) const override;
    

    void loadFromFile(std::istream& in) override;
    

    std::string getType() const override;
};

#endif // CIRCLE_H
