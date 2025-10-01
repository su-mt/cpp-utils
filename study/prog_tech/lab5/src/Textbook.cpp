#include "include/Textbook.h"

// Конструкторы
Textbook::Textbook() : Base(), title(""), author(""), year(0), institution(""), studyYear(0), pages(0) {}

Textbook::Textbook(const std::string& title, const std::string& author, int year, 
                   const std::string& institution, int studyYear, int pages, double cost) 
    : Base(cost), title(title), author(author), year(year), 
      institution(institution), studyYear(studyYear), pages(pages) {}

// Реализация чисто виртуальных методов
void Textbook::display() const {
    std::cout << "=== УЧЕБНИК ===" << std::endl;
    std::cout << "Название: " << title << std::endl;
    std::cout << "Автор: " << author << std::endl;
    std::cout << "Год выпуска: " << year << std::endl;
    std::cout << "Учебное заведение: " << institution << std::endl;
    std::cout << "Год обучения: " << studyYear << std::endl;
    std::cout << "Страниц: " << pages << std::endl;
    std::cout << "Стоимость: " << cost << " руб." << std::endl;
}

Base* Textbook::clone() const {
    return new Textbook(*this);
}

void Textbook::saveToFile(std::ofstream& file) const {
    file << "TEXTBOOK" << std::endl;
    file << title << std::endl;
    file << author << std::endl;
    file << year << std::endl;
    file << institution << std::endl;
    file << studyYear << std::endl;
    file << pages << std::endl;
    file << cost << std::endl;
}

void Textbook::loadFromFile(std::ifstream& file) {
    std::getline(file, title);
    std::getline(file, author);
    file >> year;
    file.ignore(); // игнорируем символ новой строки
    std::getline(file, institution);
    file >> studyYear >> pages >> cost;
    file.ignore(); // игнорируем символ новой строки
}

std::string Textbook::getType() const {
    return "TEXTBOOK";
}

// Геттеры
std::string Textbook::getTitle() const { return title; }
std::string Textbook::getAuthor() const { return author; }
int Textbook::getYear() const { return year; }
std::string Textbook::getInstitution() const { return institution; }
int Textbook::getStudyYear() const { return studyYear; }
int Textbook::getPages() const { return pages; }

// Сеттеры
void Textbook::setTitle(const std::string& title) { this->title = title; }
void Textbook::setAuthor(const std::string& author) { this->author = author; }
void Textbook::setYear(int year) { this->year = year; }
void Textbook::setInstitution(const std::string& institution) { this->institution = institution; }
void Textbook::setStudyYear(int studyYear) { this->studyYear = studyYear; }
void Textbook::setPages(int pages) { this->pages = pages; }

// Операторы ввода-вывода
std::ostream& operator<<(std::ostream& os, const Textbook& textbook) {
    textbook.display();
    return os;
}

std::istream& operator>>(std::istream& is, Textbook& textbook) {
    std::cout << "Введите название учебника: ";
    std::getline(is, textbook.title);
    std::cout << "Введите автора: ";
    std::getline(is, textbook.author);
    std::cout << "Введите год выпуска: ";
    is >> textbook.year;
    is.ignore(); // игнорируем символ новой строки
    std::cout << "Введите учебное заведение: ";
    std::getline(is, textbook.institution);
    std::cout << "Введите год обучения: ";
    is >> textbook.studyYear;
    std::cout << "Введите количество страниц: ";
    is >> textbook.pages;
    std::cout << "Введите стоимость: ";
    is >> textbook.cost;
    is.ignore(); // игнорируем символ новой строки
    return is;
}