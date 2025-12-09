#include <string>
#include <vector>
#include <iostream>
#include "exceptions.hpp"

namespace STUDENT {

class Student {
private:
    std::string surname;              // Фамилия и инициалы
    int group_number;                 // Номер группы
    std::vector<int> marks;           // Успеваемость (массив оценок)
    
    // Вспомогательная функция для валидации оценки
    void validateMark(int mark) const;
    
public:
    // Конструктор
    Student(const std::string& surname_initials = "", int group_number = 0);
    
    // Методы доступа (геттеры)
    std::string getSurnameInitials() const;
    int getGroupNumber() const;
    std::vector<int> getMarks() const;
    double getAverageMark() const;    // Средний балл
    
    // Методы изменения (сеттеры)
    void setSurnameInitials(const std::string& surname_initials);
    void setGroupNumber(int group_number);
    void setMarks(const std::vector<int>& marks);
    void addMark(int mark);
    
    // Операторы сравнения
    bool operator<(const Student& other) const;
    bool operator==(const Student& other) const;
    
    // Друзья для операторов потока
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);
};

// Компаратор для сортировки по среднему баллу в порядке возрастания
struct StudentComparator {
    bool operator()(const Student& a, const Student& b) const;
};

} // namespace STUDENT