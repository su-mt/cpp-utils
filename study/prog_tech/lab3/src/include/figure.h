#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>
#include <fstream>
#include <string>

/**
 * Базовый абстрактный класс "Фигура"
 */
class Figure {
public:
    /**
     * Конструктор по умолчанию
     */
    Figure() = default;
    
    /**
     * Конструктор копирования
     */
    Figure(const Figure& other) = default;
    
    /**
     * Оператор присваивания
     */
    Figure& operator=(const Figure& other) = default;
    
    /**
     * Виртуальный деструктор
     */
    virtual ~Figure() = default;
    
    /**
     * Виртуальная функция для вычисления площади фигуры
     * @return площадь фигуры
     */
    virtual double getArea() const = 0;
    
    /**
     * Виртуальная функция для вычисления периметра фигуры
     * @return периметр фигуры
     */
    virtual double getPerimeter() const = 0;
    
    /**
     * Виртуальная функция для вывода информации о фигуре на консоль
     */
    virtual void printInfo() const = 0;
    
    /**
     * Виртуальная функция для сохранения данных в файл
     * @param out поток вывода
     */
    virtual void saveToFile(std::ostream& out) const = 0;
    
    /**
     * Виртуальная функция для загрузки данных из файла
     * @param in поток ввода
     */
    virtual void loadFromFile(std::istream& in) = 0;
    
    /**
     * Виртуальная функция для получения типа фигуры
     * @return строка с типом фигуры
     */
    virtual std::string getType() const = 0;
};

#endif // FIGURE_H
