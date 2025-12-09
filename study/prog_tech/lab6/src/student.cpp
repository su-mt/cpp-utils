#include "include/student.hpp"
#include <iostream>
#include <numeric>
#include <cmath>

using namespace STUDENT;

// Вспомогательная функция для валидации оценки
void Student::validateMark(int mark) const {
    if (mark < 1 || mark > 5) {
        throw InvalidMarkException(mark);
    }
}

// Конструктор
Student::Student(const std::string& surname_initials, int group_number)
    : surname(surname_initials), group_number(group_number) {
    if (surname_initials.empty()) {
        throw EmptySurnameException();
    }
    if (group_number <= 0) {
        throw InvalidGroupNumberException(group_number);
    }
}

// Методы доступа (геттеры)
std::string Student::getSurnameInitials() const {
    return surname;
}

int Student::getGroupNumber() const {
    return group_number;
}

std::vector<int> Student::getMarks() const {
    return marks;
}

double Student::getAverageMark() const {
    if (marks.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(marks.begin(), marks.end(), 0.0);
    return sum / marks.size();
}

// Методы изменения (сеттеры)
void Student::setSurnameInitials(const std::string& surname_initials) {
    if (surname_initials.empty()) {
        throw EmptySurnameException();
    }
    surname = surname_initials;
}

void Student::setGroupNumber(int group_number_val) {
    if (group_number_val <= 0) {
        throw InvalidGroupNumberException(group_number_val);
    }
    group_number = group_number_val;
}

void Student::setMarks(const std::vector<int>& marks_val) {
    if (marks_val.empty()) {
        throw EmptyMarksException();
    }
    for (int mark : marks_val) {
        validateMark(mark);
    }
    marks = marks_val;
}

void Student::addMark(int mark) {
    validateMark(mark);
    marks.push_back(mark);
}

// Операторы сравнения
bool Student::operator<(const Student& other) const {
    // Сравниваем по среднему баллу в порядке возрастания
    double avg1 = this->getAverageMark();
    double avg2 = other.getAverageMark();
    
    if (std::fabs(avg1 - avg2) < 1e-9) {
        // Если средние баллы равны, сравниваем по фамилии
        return surname < other.surname;
    }
    return avg1 < avg2;
}

bool Student::operator==(const Student& other) const {
    return surname == other.surname && 
           group_number == other.group_number && 
           marks == other.marks;
}

// Оператор вставки (вывод)
std::ostream& STUDENT::operator<<(std::ostream& os, const Student& student) {
    os << "Фамилия и инициалы: " << student.surname << std::endl;
    os << "Номер группы: " << student.group_number << std::endl;
    os << "Оценки: ";
    
    if (student.marks.empty()) {
        os << "нет оценок";
    } else {
        for (size_t i = 0; i < student.marks.size(); ++i) {
            os << student.marks[i];
            if (i < student.marks.size() - 1) {
                os << ", ";
            }
        }
    }
    os << std::endl;
    os << "Средний балл: " << student.getAverageMark() << std::endl;
    
    return os;
}

// Оператор извлечения (ввод)
std::istream& STUDENT::operator>>(std::istream& is, Student& student) {
    std::cout << "Введите фамилию и инициалы: ";
    std::getline(is, student.surname);
    
    if (student.surname.empty()) {
        throw EmptySurnameException();
    }
    
    std::cout << "Введите номер группы: ";
    is >> student.group_number;
    is.ignore();  // Игнорируем символ новой строки после числа
    
    if (student.group_number <= 0) {
        throw InvalidGroupNumberException(student.group_number);
    }
    
    std::cout << "Введите количество оценок: ";
    int count;
    is >> count;
    is.ignore();  // Игнорируем символ новой строки
    
    if (count <= 0) {
        throw EmptyMarksException();
    }
    
    student.marks.clear();
    for (int i = 0; i < count; ++i) {
        std::cout << "Оценка " << (i + 1) << ": ";
        int mark;
        is >> mark;
        student.validateMark(mark);
        student.marks.push_back(mark);
    }
    
    return is;
}

// Реализация компаратора
bool StudentComparator::operator()(const Student& a, const Student& b) const {
    return a < b;
}
