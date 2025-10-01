#ifndef STATIONERY_H
#define STATIONERY_H

#include "Base.h"
#include <string>

/**
 * Класс Канцелярия - производный от Base
 * Поля: тип канцелярии, цвет, назначение, стоимость
 */
class Stationery : public Base {
private:
    std::string type;        // тип канцелярии
    std::string color;       // цвет
    std::string purpose;     // назначение

public:
    // Конструкторы
    Stationery();
    Stationery(const std::string& type, const std::string& color, 
               const std::string& purpose, double cost);
    
    // Деструктор
    virtual ~Stationery() = default;
    
    // Реализация чисто виртуальных методов
    virtual void display() const override;
    virtual Base* clone() const override;
    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;
    virtual std::string getType() const override;
    
    // Геттеры
    std::string getStationeryType() const;
    std::string getColor() const;
    std::string getPurpose() const;
    
    // Сеттеры
    void setStationeryType(const std::string& type);
    void setColor(const std::string& color);
    void setPurpose(const std::string& purpose);
    
    // Операторы ввода-вывода
    friend std::ostream& operator<<(std::ostream& os, const Stationery& stationery);
    friend std::istream& operator>>(std::istream& is, Stationery& stationery);
};

#endif // STATIONERY_H