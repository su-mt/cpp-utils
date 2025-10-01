#ifndef KEEPER_H
#define KEEPER_H

#include "Base.h"
#include "Book.h"
#include "Textbook.h"
#include "Stationery.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

/**
 * Класс Keeper - контейнер для хранения и обработки объектов Base
 * Обеспечивает добавление/удаление объектов, сохранение/восстановление из файла
 */
class Keeper {
private:
    std::vector<std::unique_ptr<Base>> items; // контейнер для хранения объектов

public:
    // Конструктор и деструктор
    Keeper();
    ~Keeper() = default;
    
    // Запрет копирования (можно реализовать при необходимости)
    Keeper(const Keeper&) = delete;
    Keeper& operator=(const Keeper&) = delete;
    
    // Методы для работы с объектами
    void addItem(std::unique_ptr<Base> item);
    void addBook(const Book& book);
    void addTextbook(const Textbook& textbook);
    void addStationery(const Stationery& stationery);
    
    bool removeItem(size_t index);
    void removeAllItems();
    
    // Методы для отображения
    void displayAll() const;
    void displayItem(size_t index) const;
    size_t getSize() const;
    bool isEmpty() const;
    
    // Методы для работы с файлами
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    // Поиск и фильтрация
    void displayByType(const std::string& type) const;
    std::vector<size_t> findByType(const std::string& type) const;
    
    // Операторы доступа
    Base* operator[](size_t index);
    const Base* operator[](size_t index) const;
    
    // Операторы ввода-вывода
    friend std::ostream& operator<<(std::ostream& os, const Keeper& keeper);
};

#endif // KEEPER_H