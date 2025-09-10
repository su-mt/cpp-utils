#include "include/circle.h"
#include <iomanip>
#include <stdexcept>

Circle::Circle() : radius(1.0) {
}

Circle::Circle(double r) : radius(r) {
    if (radius <= 0) {
        throw std::invalid_argument("Радиус должен быть положительным числом");
    }
}

Circle::Circle(const Circle& other) : radius(other.radius) {
}

Circle& Circle::operator=(const Circle& other) {
    if (this != &other) {
        radius = other.radius;
    }
    return *this;
}

Circle::~Circle() {
}

double Circle::getRadius() const {
    return radius;
}

double Circle::getArea() const {
    return M_PI * radius * radius;
}

double Circle::getPerimeter() const {
    return 2 * M_PI * radius;
}

void Circle::printInfo() const {
    std::cout << "=== КРУГ ===" << std::endl;
    std::cout << "Радиус: " << std::fixed << std::setprecision(2) << radius << std::endl;
    std::cout << "Площадь: " << std::fixed << std::setprecision(2) << getArea() << std::endl;
    std::cout << "Периметр (длина окружности): " << std::fixed << std::setprecision(2) << getPerimeter() << std::endl;
    std::cout << std::endl;
}

void Circle::saveToFile(std::ostream& out) const {
    out << "Circle " << radius << std::endl;
}

void Circle::loadFromFile(std::istream& in) {
    in >> radius;
    if (radius <= 0) {
        throw std::invalid_argument("Радиус должен быть положительным числом");
    }
}

std::string Circle::getType() const {
    return "Circle";
}
