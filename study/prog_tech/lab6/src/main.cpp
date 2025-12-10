#include "include/student.hpp"
#include "include/exceptions.hpp"
#include <set>
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <string>

using STUDENT::Student;
using STUDENT::StudentComparator;
using STUDENT::StudentException;
using STUDENT::EmptySurnameException;
using STUDENT::InvalidGroupNumberException;
using STUDENT::EmptyMarksException;



std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}


std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return trim(input);
}


int readInt(const std::string& prompt) {
    std::string input = readLine(prompt);
    return std::stoi(input);
}


Student inputStudent() {

    std::string surname = readLine("Введите фамилию и инициалы: ");
    if (surname.empty()) {
        throw EmptySurnameException();
    }
    

    int groupNumber = readInt("Введите номер группы: ");
    if (groupNumber <= 0) {
        throw InvalidGroupNumberException(groupNumber);
    }
    
    Student student(surname, groupNumber);
    
    int count = readInt("Введите количество оценок: ");
    if (count <= 0) {
        throw EmptyMarksException();
    }
    
    for (int i = 0; i < count; ++i) {
        int mark = readInt("Оценка " + std::to_string(i + 1) + ": ");
        student.addMark(mark);
    }
    
    return student;
}

int main() {
    std::set<Student, StudentComparator> students;
    int choice;
    
    std::cout << "=== Программа управления студентами ===" << std::endl;
    
    while (true) {
        try {
            std::cout << "\nМеню:" << std::endl;
            std::cout << "1. Добавить студента" << std::endl;
            std::cout << "2. Вывести всех студентов (отсортировано по среднему баллу)" << std::endl;
            std::cout << "3. Вывести студентов с оценками 4 и 5" << std::endl;
            std::cout << "4. Выход" << std::endl;
            std::cout << "Выбор: ";
            
            std::string choiceStr;
            if (!std::getline(std::cin, choiceStr)) {
                break; // Выход при EOF
            }
            
            try {
                choice = std::stoi(choiceStr);
            } catch (...) {
                choice = 0; // Неверный ввод
            }
            
            if (choice == 1) {
                try {
                    Student student = inputStudent();
                    students.insert(student);
                    std::cout << "Студент добавлен успешно!" << std::endl;
                } catch (const StudentException& e) {
                    std::cerr << "\n" << e.what() << std::endl;
                    std::cout << "Пожалуйста, попробуйте снова." << std::endl;
                }
                
            } else if (choice == 2) {
                if (students.empty()) {
                    std::cout << "Нет студентов в базе данных." << std::endl;
                } else {
                    std::cout << "\n=== Список всех студентов (отсортировано по среднему баллу) ===" << std::endl;
                    std::cout << std::string(70, '-') << std::endl;
                    for (const auto& student : students) {
                        std::cout << student << std::string(70, '-') << std::endl;
                    }
                }
                
            } else if (choice == 3) {

                std::vector<std::pair<std::string, int>> excellentStudents;
                
                for (const auto& student : students) {
                    const auto& marks = student.getMarks();
                    bool hasOnlyHighMarks = true;
                    

                    for (int mark : marks) {
                        if (mark < 4) {
                            hasOnlyHighMarks = false;
                            break;
                        }
                    }
                    
                    if (hasOnlyHighMarks && !marks.empty()) {
                        excellentStudents.push_back({
                            student.getSurnameInitials(),
                            student.getGroupNumber()
                        });
                    }
                }
                
                if (excellentStudents.empty()) {
                    std::cout << "\nНет студентов, имеющих только оценки 4 и 5." << std::endl;
                } else {
                    std::cout << "\n=== Студенты с оценками 4 и 5 ===" << std::endl;
                    std::cout << std::string(50, '-') << std::endl;
                    std::cout << std::left << std::setw(30) << "Фамилия и инициалы" 
                              << std::setw(15) << "Номер группы" << std::endl;
                    std::cout << std::string(50, '-') << std::endl;
                    
                    for (const auto& student : excellentStudents) {
                        std::cout << std::left << std::setw(30) << student.first 
                                  << std::setw(15) << student.second << std::endl;
                    }
                    std::cout << std::string(50, '-') << std::endl;
                }
                
            } else if (choice == 4) {
                std::cout << "Выход из программы." << std::endl;
                break;
                
            } else {
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "\nНепредвиденная ошибка: " << e.what() << std::endl;
        }
    }
    
    return 0;
}

