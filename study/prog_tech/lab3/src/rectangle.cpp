#include "include/rectangle.h"
#include <iomanip>
#include <stdexcept>

Rectangle::Rectangle() : width(1.0), height(1.0) {
    // Конструктор по умолчанию создает квадрат 1x1
}

Rectangle::Rectangle(double w, double h) : width(w), height(h) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Ширина и высота должны быть положительными числами");
    }
}

Rectangle::Rectangle(const Rectangle& other) : width(other.width), height(other.height) {
    // Конструктор копирования
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
    }
    return *this;
}

Rectangle::~Rectangle() {
    // Виртуальный деструктор
}

double Rectangle::getWidth() const {
    return width;
}

double Rectangle::getHeight() const {
    return height;
}

double Rectangle::getArea() const {
    return width * height;
}

double Rectangle::getPerimeter() const {
    return 2 * (width + height);
}

void Rectangle::printInfo() const {
    std::cout << "=== ПРЯМОУГОЛЬНИК ===" << std::endl;
    std::cout << "Ширина: " << std::fixed << std::setprecision(2) << width << std::endl;
    std::cout << "Высота: " << std::fixed << std::setprecision(2) << height << std::endl;
    std::cout << "Площадь: " << std::fixed << std::setprecision(2) << getArea() << std::endl;
    std::cout << "Периметр: " << std::fixed << std::setprecision(2) << getPerimeter() << std::endl;
    std::cout << std::endl;
}

void Rectangle::saveToFile(std::ostream& out) const {
    out << "Rectangle " << width << " " << height << std::endl;
}

void Rectangle::loadFromFile(std::istream& in) {
    in >> width >> height;
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Ширина и высота должны быть положительными числами");
    }
}

std::string Rectangle::getType() const {
    return "Rectangle";
}
