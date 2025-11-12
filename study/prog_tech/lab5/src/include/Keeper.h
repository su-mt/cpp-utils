#ifndef KEEPER_H
#define KEEPER_H

#include "Base.h"
#include "Book.h"
#include "Textbook.h"
#include "Stationery.h"
#include "my_vector.h"
#include <memory>
#include <string>
#include <fstream>

class Keeper {
private:
    my_vector<std::unique_ptr<Base>> items;

public:
    Keeper();
    ~Keeper() = default;
    
    // Запрет копирования
    Keeper(const Keeper&) = delete;
    Keeper& operator=(const Keeper&) = delete;
    void addItem(std::unique_ptr<Base> item);
    void addBook(const Book& book);
    void addTextbook(const Textbook& textbook);
    void addStationery(const Stationery& stationery);
    
    bool removeItem(size_t index);
    void removeAllItems();
    
    void displayAll() const;
    void displayItem(size_t index) const;
    size_t getSize() const;
    bool isEmpty() const;
    
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    void displayByType(const std::string& type) const;
    my_vector<size_t> findByType(const std::string& type) const;
    
    Base* operator[](size_t index);
    const Base* operator[](size_t index) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Keeper& keeper);
};

#endif // KEEPER_H