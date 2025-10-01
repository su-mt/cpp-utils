#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <fstream>
#include <string>

/**
 * Абстрактный базовый класс для всех объектов в книжном магазине
 */
class Base {
protected:
    double cost;  // стоимость

public:
    // Конструкторы
    Base();
    Base(double cost);
    
    // Виртуальный деструктор
    virtual ~Base() = default;
    
    // Чисто виртуальные методы
    virtual void display() const = 0;
    virtual Base* clone() const = 0;
    virtual void saveToFile(std::ofstream& file) const = 0;
    virtual void loadFromFile(std::ifstream& file) = 0;
    virtual std::string getType() const = 0;
    
    // Геттеры и сеттеры
    double getCost() const;
    void setCost(double cost);
    
    // Операторы ввода-вывода
    friend std::ostream& operator<<(std::ostream& os, const Base& obj);
    friend std::istream& operator>>(std::istream& is, Base& obj);
};

#endif // BASE_H