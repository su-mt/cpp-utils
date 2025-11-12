#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <fstream>
#include <string>


class Base {
protected:
    double cost; 

public:

    Base();
    Base(double cost);
    

    virtual ~Base() = default;
    

    virtual void display() const = 0;
    virtual Base* clone() const = 0;
    virtual void saveToFile(std::ofstream& file) const = 0;
    virtual void loadFromFile(std::ifstream& file) = 0;
    virtual std::string getType() const = 0;
    

    double getCost() const;
    void setCost(double cost);
    

    friend std::ostream& operator<<(std::ostream& os, const Base& obj);
    friend std::istream& operator>>(std::istream& is, Base& obj);
};

#endif // BASE_H