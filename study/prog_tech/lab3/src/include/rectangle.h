#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"


class Rectangle : public Figure {
private:
    double width;
    double height;

public:

    Rectangle();
    
    Rectangle(double w, double h);
    
    Rectangle(const Rectangle& other);
    
    Rectangle& operator=(const Rectangle& other);
    
    virtual ~Rectangle();
    
    double getWidth() const;
    
    double getHeight() const;
    
    double getArea() const override;
    
    double getPerimeter() const override;
    
    void printInfo() const override;
    
    void saveToFile(std::ostream& out) const override;
    
    void loadFromFile(std::istream& in) override;
    
    std::string getType() const override;
};

#endif // RECTANGLE_H
