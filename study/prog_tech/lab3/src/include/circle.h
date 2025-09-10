#ifndef CIRCLE_H
#define CIRCLE_H

#include "figure.h"
#include <cmath>

/**
 * Класс "Круг", наследующий от базового класса Figure
 */
class Circle : public Figure {
private:
    double radius;

public:
    /**
     * Конструктор по умолчанию
     */
    Circle();
    
    /**
     * Конструктор с параметром
     * @param r радиус круга
     */
    explicit Circle(double r);
    
    /**
     * Конструктор копирования
     * @param other копируемый объект
     */
    Circle(const Circle& other);
    
    /**
     * Оператор присваивания
     * @param other объект для присваивания
     * @return ссылка на текущий объект
     */
    Circle& operator=(const Circle& other);
    
    /**
     * Виртуальный деструктор
     */
    virtual ~Circle();
    
    /**
     * Получить радиус круга
     * @return радиус
     */
    double getRadius() const;
    
    /**
     * Вычисление площади круга
     * @return площадь круга (π * r²)
     */
    double getArea() const override;
    
    /**
     * Вычисление периметра (длины окружности)
     * @return периметр круга (2 * π * r)
     */
    double getPerimeter() const override;
    
    /**
     * Вывод информации о круге на консоль
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
     * @return строка "Circle"
     */
    std::string getType() const override;
};

#endif // CIRCLE_H
