#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include <fstream>
#include <string>



class Figure {
public:

    Figure() = default;
    

    Figure(const Figure& other) = default;
    

    Figure& operator=(const Figure& other) = default;
    

    virtual ~Figure() = default;
    
    virtual double getArea() const = 0;
    

    virtual double getPerimeter() const = 0;
    

    virtual void printInfo() const = 0;
    

    virtual void saveToFile(std::ostream& out) const = 0;
    

    virtual void loadFromFile(std::istream& in) = 0;
    

    virtual std::string getType() const = 0;
};

#endif // FIGURE_H
