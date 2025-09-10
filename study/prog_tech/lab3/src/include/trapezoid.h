#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "figure.h"
#include <cmath>

/**
 * Класс "Трапеция", наследующий от базового класса Figure
 */
class Trapezoid : public Figure {
private:
    double base1;    // верхнее основание
    double base2;    // нижнее основание
    double height;   // высота
    double side1;    // боковая сторона 1
    double side2;    // боковая сторона 2

public:
    /**
     * Конструктор по умолчанию
     */
    Trapezoid();
    
    /**
     * Конструктор с параметрами
     * @param b1 верхнее основание
     * @param b2 нижнее основание
     * @param h высота
     * @param s1 боковая сторона 1
     * @param s2 боковая сторона 2
     */
    Trapezoid(double b1, double b2, double h, double s1, double s2);
    
    /**
     * Конструктор копирования
     * @param other копируемый объект
     */
    Trapezoid(const Trapezoid& other);
    
    /**
     * Оператор присваивания
     * @param other объект для присваивания
     * @return ссылка на текущий объект
     */
    Trapezoid& operator=(const Trapezoid& other);
    
    /**
     * Виртуальный деструктор
     */
    virtual ~Trapezoid();
    
    /**
     * Получить верхнее основание
     * @return верхнее основание
     */
    double getBase1() const;
    
    /**
     * Получить нижнее основание
     * @return нижнее основание
     */
    double getBase2() const;
    
    /**
     * Получить высоту
     * @return высота
     */
    double getHeight() const;
    
    /**
     * Вычисление площади трапеции
     * @return площадь трапеции ((base1 + base2) * height / 2)
     */
    double getArea() const override;
    
    /**
     * Вычисление периметра трапеции
     * @return периметр трапеции (base1 + base2 + side1 + side2)
     */
    double getPerimeter() const override;
    
    /**
     * Вывод информации о трапеции на консоль
     */
    void printInfo() const override;
    
    /**
     * Сохранение данных в файл
     * @param out поток вывода
     */
    void saveToFile(std::ostream& out) const override;
    
    /**
     * Загрузка данных из файла
     * @param in поток ввода
     */
    void loadFromFile(std::istream& in) override;
    
    /**
     * Получение типа фигуры
     * @return строка "Trapezoid"
     */
    std::string getType() const override;
};

#endif // TRAPEZOID_H
