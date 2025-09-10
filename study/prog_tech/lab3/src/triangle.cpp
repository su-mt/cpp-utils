#include "include/triangle.h"
#include <iomanip>
#include <stdexcept>

Triangle::Triangle() : side1(3.0), side2(4.0), side3(5.0) {

}

Triangle::Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {
    if (side1 <= 0 || side2 <= 0 || side3 <= 0) {
        throw std::invalid_argument("Все стороны треугольника должны быть положительными числами");
    }
    if (!isValid()) {
        throw std::invalid_argument("Треугольник с такими сторонами не может существовать");
    }
}

Triangle::Triangle(const Triangle& other) : side1(other.side1), side2(other.side2), side3(other.side3) {

}

Triangle& Triangle::operator=(const Triangle& other) {
    if (this != &other) {
        side1 = other.side1;
        side2 = other.side2;
        side3 = other.side3;
    }
    return *this;
}

Triangle::~Triangle() {

}

double Triangle::getSide1() const {
    return side1;
}

double Triangle::getSide2() const {
    return side2;
}

double Triangle::getSide3() const {
    return side3;
}

bool Triangle::isValid() const {

    return (side1 + side2 > side3) && 
           (side1 + side3 > side2) && 
           (side2 + side3 > side1);
}

double Triangle::getArea() const {

    double s = getPerimeter() / 2.0; 
    return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
}

double Triangle::getPerimeter() const {
    return side1 + side2 + side3;
}

void Triangle::printInfo() const {
    std::cout << "=== ТРЕУГОЛЬНИК ===" << std::endl;
    std::cout << "Сторона 1: " << std::fixed << std::setprecision(2) << side1 << std::endl;
    std::cout << "Сторона 2: " << std::fixed << std::setprecision(2) << side2 << std::endl;
    std::cout << "Сторона 3: " << std::fixed << std::setprecision(2) << side3 << std::endl;
    std::cout << "Площадь: " << std::fixed << std::setprecision(2) << getArea() << std::endl;
    std::cout << "Периметр: " << std::fixed << std::setprecision(2) << getPerimeter() << std::endl;
    std::cout << std::endl;
}

void Triangle::saveToFile(std::ostream& out) const {
    out << "Triangle " << side1 << " " << side2 << " " << side3 << std::endl;
}

void Triangle::loadFromFile(std::istream& in) {
    in >> side1 >> side2 >> side3;
    if (side1 <= 0 || side2 <= 0 || side3 <= 0) {
        throw std::invalid_argument("Все стороны треугольника должны быть положительными числами");
    }
    if (!isValid()) {
        throw std::invalid_argument("Треугольник с такими сторонами не может существовать");
    }
}

std::string Triangle::getType() const {
    return "Triangle";
}
