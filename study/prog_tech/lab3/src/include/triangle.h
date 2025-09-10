#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "figure.h"
#include <cmath>

/**
 * Класс "Треугольник", наследующий от базового класса Figure
 */
class Triangle : public Figure {
private:
    double side1;
    double side2;
    double side3;

public:
    /**
     * Конструктор по умолчанию
     */
    Triangle();
    
    /**
     * Конструктор с параметрами
     * @param s1 сторона 1
     * @param s2 сторона 2
     * @param s3 сторона 3
     */
    Triangle(double s1, double s2, double s3);
    
    /**
     * Конструктор копирования
     * @param other копируемый объект
     */
    Triangle(const Triangle& other);
    
    /**
     * Оператор присваивания
     * @param other объект для присваивания
     * @return ссылка на текущий объект
     */
    Triangle& operator=(const Triangle& other);
    
    /**
     * Виртуальный деструктор
     */
    virtual ~Triangle();
    
    /**
     * Получить сторону 1
     * @return сторона 1
     */
    double getSide1() const;
    
    /**
     * Получить сторону 2
     * @return сторона 2
     */
    double getSide2() const;
    
    /**
     * Получить сторону 3
     * @return сторона 3
     */
    double getSide3() const;
    
    /**
     * Проверка валидности треугольника
     * @return true если треугольник валиден
     */
    bool isValid() const;
    
    /**
     * Вычисление площади треугольника (формула Герона)
     * @return площадь треугольника
     */
    double getArea() const override;
    
    /**
     * Вычисление периметра треугольника
     * @return периметр треугольника (side1 + side2 + side3)
     */
    double getPerimeter() const override;
    
    /**
     * Вывод информации о треугольнике на консоль
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
     * @return строка "Triangle"
     */
    std::string getType() const override;
};

#endif // TRIANGLE_H
