#ifndef BOOK_H
#define BOOK_H

#include "Base.h"
#include <string>

/**
 * Класс Книга - производный от Base
 * Поля: название, автор, год выпуска, аннотация, жанр, объем страниц, стоимость
 */
class Book : public Base {
private:
    std::string title;        // название
    std::string author;       // автор
    int year;                // год выпуска
    std::string annotation;   // аннотация
    std::string genre;        // жанр
    int pages;               // объем страниц

public:
    // Конструкторы
    Book();
    Book(const std::string& title, const std::string& author, int year, 
         const std::string& annotation, const std::string& genre, 
         int pages, double cost);
    
    // Деструктор
    virtual ~Book() = default;
    
    // Реализация чисто виртуальных методов
    virtual void display() const override;
    virtual Base* clone() const override;
    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;
    virtual std::string getType() const override;
    
    // Геттеры
    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getAnnotation() const;
    std::string getGenre() const;
    int getPages() const;
    
    // Сеттеры
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setAnnotation(const std::string& annotation);
    void setGenre(const std::string& genre);
    void setPages(int pages);
    
    // Операторы ввода-вывода
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
    friend std::istream& operator>>(std::istream& is, Book& book);
};

#endif // BOOK_H