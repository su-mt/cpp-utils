#include "include/student.hpp"
#include "include/exceptions.hpp"
#include <set>
#include <iostream>
#include <iomanip>
#include <vector>

using STUDENT::Student;
using STUDENT::StudentComparator;
using STUDENT::StudentException;

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
            
            std::cin >> choice;
            std::cin.ignore();  // Очищаем буфер
            
            if (choice == 1) {
                // Добавление студента
                try {
                    Student student;
                    std::cin >> student;
                    students.insert(student);
                    std::cout << "✓ Студент добавлен успешно!" << std::endl;
                } catch (const StudentException& e) {
                    std::cerr << "\n❌ " << e.what() << std::endl;
                    std::cout << "Пожалуйста, попробуйте снова." << std::endl;
                }
                
            } else if (choice == 2) {
                // Вывод всех студентов
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
                // Вывод студентов с оценками 4 и 5
                std::vector<std::pair<std::string, int>> excellentStudents;
                
                for (const auto& student : students) {
                    const auto& marks = student.getMarks();
                    bool hasOnlyHighMarks = true;
                    
                    // Проверяем, что все оценки >= 4
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
            std::cerr << "\n❌ Непредвиденная ошибка: " << e.what() << std::endl;
        }
    }
    
    return 0;
}

