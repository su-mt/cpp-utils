#include "include/trapezoid.h"
#include <iomanip>
#include <stdexcept>

Trapezoid::Trapezoid() : base1(1.0), base2(2.0), height(1.0), side1(1.5), side2(1.5) {

}

Trapezoid::Trapezoid(double b1, double b2, double h, double s1, double s2) 
    : base1(b1), base2(b2), height(h), side1(s1), side2(s2) {
    if (base1 <= 0 || base2 <= 0 || height <= 0 || side1 <= 0 || side2 <= 0) {
        throw std::invalid_argument("Все параметры трапеции должны быть положительными числами");
    }
}

Trapezoid::Trapezoid(const Trapezoid& other) 
    : base1(other.base1), base2(other.base2), height(other.height), 
      side1(other.side1), side2(other.side2) {

}

Trapezoid& Trapezoid::operator=(const Trapezoid& other) {
    if (this != &other) {
        base1 = other.base1;
        base2 = other.base2;
        height = other.height;
        side1 = other.side1;
        side2 = other.side2;
    }
    return *this;
}

Trapezoid::~Trapezoid() {

}

double Trapezoid::getBase1() const {
    return base1;
}

double Trapezoid::getBase2() const {
    return base2;
}

double Trapezoid::getHeight() const {
    return height;
}

double Trapezoid::getArea() const {
    return (base1 + base2) * height / 2.0;
}

double Trapezoid::getPerimeter() const {
    return base1 + base2 + side1 + side2;
}

void Trapezoid::printInfo() const {
    std::cout << "=== ТРАПЕЦИЯ ===" << std::endl;
    std::cout << "Верхнее основание: " << std::fixed << std::setprecision(2) << base1 << std::endl;
    std::cout << "Нижнее основание: " << std::fixed << std::setprecision(2) << base2 << std::endl;
    std::cout << "Высота: " << std::fixed << std::setprecision(2) << height << std::endl;
    std::cout << "Боковая сторона 1: " << std::fixed << std::setprecision(2) << side1 << std::endl;
    std::cout << "Боковая сторона 2: " << std::fixed << std::setprecision(2) << side2 << std::endl;
    std::cout << "Площадь: " << std::fixed << std::setprecision(2) << getArea() << std::endl;
    std::cout << "Периметр: " << std::fixed << std::setprecision(2) << getPerimeter() << std::endl;
    std::cout << std::endl;
}

void Trapezoid::saveToFile(std::ostream& out) const {
    out << "Trapezoid " << base1 << " " << base2 << " " << height << " " << side1 << " " << side2 << std::endl;
}

void Trapezoid::loadFromFile(std::istream& in) {
    in >> base1 >> base2 >> height >> side1 >> side2;
    if (base1 <= 0 || base2 <= 0 || height <= 0 || side1 <= 0 || side2 <= 0) {
        throw std::invalid_argument("Все параметры трапеции должны быть положительными числами");
    }
}

std::string Trapezoid::getType() const {
    return "Trapezoid";
}
