#include "include/Stationery.h"

// Конструкторы
Stationery::Stationery() : Base(), type(""), color(""), purpose("") {}

Stationery::Stationery(const std::string& type, const std::string& color, 
                       const std::string& purpose, double cost) 
    : Base(cost), type(type), color(color), purpose(purpose) {}


void Stationery::display() const {
    std::cout << "=== КАНЦЕЛЯРИЯ ===" << std::endl;
    std::cout << "Тип: " << type << std::endl;
    std::cout << "Цвет: " << color << std::endl;
    std::cout << "Назначение: " << purpose << std::endl;
    std::cout << "Стоимость: " << cost << " руб." << std::endl;
}

Base* Stationery::clone() const {
    return new Stationery(*this);
}

void Stationery::saveToFile(std::ofstream& file) const {
    file << "STATIONERY" << std::endl;
    file << type << std::endl;
    file << color << std::endl;
    file << purpose << std::endl;
    file << cost << std::endl;
}

void Stationery::loadFromFile(std::ifstream& file) {
    std::getline(file, type);
    std::getline(file, color);
    std::getline(file, purpose);
    file >> cost;
    file.ignore(); 
}

std::string Stationery::getType() const {
    return "STATIONERY";
}


std::string Stationery::getStationeryType() const { return type; }
std::string Stationery::getColor() const { return color; }
std::string Stationery::getPurpose() const { return purpose; }


void Stationery::setStationeryType(const std::string& type) { this->type = type; }
void Stationery::setColor(const std::string& color) { this->color = color; }
void Stationery::setPurpose(const std::string& purpose) { this->purpose = purpose; }


std::ostream& operator<<(std::ostream& os, const Stationery& stationery) {
    stationery.display();
    return os;
}

std::istream& operator>>(std::istream& is, Stationery& stationery) {
    std::cout << "Введите тип канцелярии: ";
    std::getline(is, stationery.type);
    std::cout << "Введите цвет: ";
    std::getline(is, stationery.color);
    std::cout << "Введите назначение: ";
    std::getline(is, stationery.purpose);
    std::cout << "Введите стоимость: ";
    is >> stationery.cost;
    is.ignore(); 
    return is;
}