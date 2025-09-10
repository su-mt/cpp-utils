#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "figure.h"
#include <cmath>


class Trapezoid : public Figure {
private:
    double base1;    
    double base2;  
    double height;  
    double side1;   
    double side2;   

public:

    Trapezoid();
    Trapezoid(double b1, double b2, double h, double s1, double s2);
    
    Trapezoid(const Trapezoid& other);
    
    Trapezoid& operator=(const Trapezoid& other);
    
    virtual ~Trapezoid();
    
    double getBase1() const;
    
    double getBase2() const;
    
    double getHeight() const;
    
    double getArea() const override;
    
    double getPerimeter() const override;
    
    void printInfo() const override;
    
    void saveToFile(std::ostream& out) const override;
    
    void loadFromFile(std::istream& in) override;
    
    std::string getType() const override;
};

#endif // TRAPEZOID_H
