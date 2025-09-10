#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "figure.h"
#include <cmath>


class Triangle : public Figure {
private:
    double side1;
    double side2;
    double side3;

public:

    Triangle();
    
    Triangle(double s1, double s2, double s3);
    
    Triangle(const Triangle& other);
    
    Triangle& operator=(const Triangle& other);
    
    virtual ~Triangle();
    
    double getSide1() const;
    
    double getSide2() const;
    
    double getSide3() const;
    
    bool isValid() const;
    
    double getArea() const override;
    
    double getPerimeter() const override;
    
    void printInfo() const override;
    
    void saveToFile(std::ostream& out) const override;
    
    void loadFromFile(std::istream& in) override;
    
    std::string getType() const override;
};

#endif // TRIANGLE_H
