#ifndef BOOK_H
#define BOOK_H

#include "Base.h"
#include <string>


class Book : public Base {
private:
    std::string title;       
    std::string author;      
    int year;                
    std::string annotation;  
    std::string genre;        
    int pages;               

public:

    Book();
    Book(const std::string& title, const std::string& author, int year, 
         const std::string& annotation, const std::string& genre, 
         int pages, double cost);
    

    virtual ~Book() = default;
    

    virtual void display() const override;
    virtual Base* clone() const override;
    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;
    virtual std::string getType() const override;
    

    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getAnnotation() const;
    std::string getGenre() const;
    int getPages() const;
    

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setAnnotation(const std::string& annotation);
    void setGenre(const std::string& genre);
    void setPages(int pages);
    

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
    friend std::istream& operator>>(std::istream& is, Book& book);
};

#endif // BOOK_H