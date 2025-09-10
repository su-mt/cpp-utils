#ifndef MENU_H
#define MENU_H

#include "figure.h"
#include "circle.h"
#include "rectangle.h"
#include "trapezoid.h"
#include "triangle.h"
#include <vector>
#include <memory>
#include <string>

/**
 * Класс для работы с интерактивным меню
 */
class Menu {
private:
    std::vector<std::unique_ptr<Figure>> figures;
    
    /**
     * Отображение главного меню
     */
    void showMainMenu() const;
    
    /**
     * Добавление новой фигуры
     */
    void addFigure();
    
    /**
     * Создание круга
     */
    void createCircle();
    
    /**
     * Создание прямоугольника
     */
    void createRectangle();
    
    /**
     * Создание трапеции
     */
    void createTrapezoid();
    
    /**
     * Создание треугольника
     */
    void createTriangle();
    
    /**
     * Отображение всех фигур
     */
    void showAllFigures() const;
    
    /**
     * Отображение конкретной фигуры
     */
    void showFigure();
    
    /**
     * Удаление фигуры
     */
    void deleteFigure();
    
    /**
     * Сохранение всех фигур в файл
     */
    void saveToFile() const;
    
    /**
     * Загрузка фигур из файла
     */
    void loadFromFile();
    
    /**
     * Сохранение результатов вычислений в файл
     */
    void saveResultsToFile() const;
    
    /**
     * Получение безопасного ввода числа
     */
    double getValidDouble(const std::string& prompt) const;
    
    /**
     * Получение безопасного ввода целого числа
     */
    int getValidInt(const std::string& prompt) const;
    
    /**
     * Получение имени файла
     */
    std::string getFileName(const std::string& prompt) const;
    
public:
    /**
     * Запуск главного цикла меню
     */
    void run();
};

#endif // MENU_H
