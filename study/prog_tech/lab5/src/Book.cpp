#include "include/Book.h"

Book::Book() : Base(), title(""), author(""), year(0), annotation(""), genre(""), pages(0) {}

Book::Book(const std::string& title, const std::string& author, int year, 
           const std::string& annotation, const std::string& genre, 
           int pages, double cost) 
    : Base(cost), title(title), author(author), year(year), 
      annotation(annotation), genre(genre), pages(pages) {}

void Book::display() const {
    std::cout << "=== КНИГА ===" << std::endl;
    std::cout << "Название: " << title << std::endl;
    std::cout << "Автор: " << author << std::endl;
    std::cout << "Год выпуска: " << year << std::endl;
    std::cout << "Аннотация: " << annotation << std::endl;
    std::cout << "Жанр: " << genre << std::endl;
    std::cout << "Страниц: " << pages << std::endl;
    std::cout << "Стоимость: " << cost << " руб." << std::endl;
}

Base* Book::clone() const {
    return new Book(*this);
}

void Book::saveToFile(std::ofstream& file) const {
    file << "BOOK" << std::endl;
    file << title << std::endl;
    file << author << std::endl;
    file << year << std::endl;
    file << annotation << std::endl;
    file << genre << std::endl;
    file << pages << std::endl;
    file << cost << std::endl;
}

void Book::loadFromFile(std::ifstream& file) {
    std::getline(file, title);
    std::getline(file, author);
    file >> year;
    file.ignore(); 
    std::getline(file, annotation);
    std::getline(file, genre);
    file >> pages >> cost;
    file.ignore(); 
}

std::string Book::getType() const {
    return "BOOK";
}

// Геттеры
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
std::string Book::getAnnotation() const { return annotation; }
std::string Book::getGenre() const { return genre; }
int Book::getPages() const { return pages; }

// Сеттеры
void Book::setTitle(const std::string& title) { this->title = title; }
void Book::setAuthor(const std::string& author) { this->author = author; }
void Book::setYear(int year) { this->year = year; }
void Book::setAnnotation(const std::string& annotation) { this->annotation = annotation; }
void Book::setGenre(const std::string& genre) { this->genre = genre; }
void Book::setPages(int pages) { this->pages = pages; }


std::ostream& operator<<(std::ostream& os, const Book& book) {
    book.display();
    return os;
}

std::istream& operator>>(std::istream& is, Book& book) {
    std::cout << "Введите название книги: ";
    std::getline(is, book.title);
    std::cout << "Введите автора: ";
    std::getline(is, book.author);
    std::cout << "Введите год выпуска: ";
    is >> book.year;
    is.ignore(); 
    std::cout << "Введите аннотацию: ";
    std::getline(is, book.annotation);
    std::cout << "Введите жанр: ";
    std::getline(is, book.genre);
    std::cout << "Введите количество страниц: ";
    is >> book.pages;
    std::cout << "Введите стоимость: ";
    is >> book.cost;
    is.ignore(); 
    return is;
}