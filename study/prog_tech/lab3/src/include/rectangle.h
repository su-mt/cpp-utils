#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"

/**
 * Класс "Прямоугольник", наследующий от базового класса Figure
 */
class Rectangle : public Figure {
private:
    double width;
    double height;

public:
    /**
     * Конструктор по умолчанию
     */
    Rectangle();
    
    /**
     * Конструктор с параметрами
     * @param w ширина
     * @param h высота
     */
    Rectangle(double w, double h);
    
    /**
     * Конструктор копирования
     * @param other копируемый объект
     */
    Rectangle(const Rectangle& other);
    
    /**
     * Оператор присваивания
     * @param other объект для присваивания
     * @return ссылка на текущий объект
     */
    Rectangle& operator=(const Rectangle& other);
    
    /**
     * Виртуальный деструктор
     */
    virtual ~Rectangle();
    
    /**
     * Получить ширину прямоугольника
     * @return ширина
     */
    double getWidth() const;
    
    /**
     * Получить высоту прямоугольника
     * @return высота
     */
    double getHeight() const;
    
    /**
     * Вычисление площади прямоугольника
     * @return площадь прямоугольника (width * height)
     */
    double getArea() const override;
    
    /**
     * Вычисление периметра прямоугольника
     * @return периметр прямоугольника (2 * (width + height))
     */
    double getPerimeter() const override;
    
    /**
     * Вывод информации о прямоугольнике на консоль
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
     * @return строка "Rectangle"
     */
    std::string getType() const override;
};

#endif // RECTANGLE_H
