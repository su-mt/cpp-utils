#include "include/menu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

void Menu::showMainMenu() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "         ПРОГРАММА ДЛЯ РАБОТЫ С ФИГУРАМИ" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "1.  Добавить фигуру" << std::endl;
    std::cout << "2.  Показать все фигуры" << std::endl;
    std::cout << "3.  Показать конкретную фигуру" << std::endl;
    std::cout << "4.  Удалить фигуру" << std::endl;
    std::cout << "5.  Сохранить фигуры в файл" << std::endl;
    std::cout << "6.  Загрузить фигуры из файла" << std::endl;
    std::cout << "7.  Сохранить результаты в файл" << std::endl;
    std::cout << "8.  Очистить все фигуры" << std::endl;
    std::cout << "9.  Показать статистику" << std::endl;
    std::cout << "0.  Выход" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Количество фигур: " << figures.size() << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void Menu::addFigure() {
    std::cout << "\nВыберите тип фигуры:" << std::endl;
    std::cout << "1. Круг" << std::endl;
    std::cout << "2. Прямоугольник" << std::endl;
    std::cout << "3. Трапеция" << std::endl;
    std::cout << "4. Треугольник" << std::endl;
    std::cout << "0. Отмена" << std::endl;
    
    int choice = getValidInt("Ваш выбор: ");
    
    try {
        switch (choice) {
            case 1: createCircle(); break;
            case 2: createRectangle(); break;
            case 3: createTrapezoid(); break;
            case 4: createTriangle(); break;
            case 0: return;
            default:
                std::cout << "Неверный выбор!" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка при создании фигуры: " << e.what() << std::endl;
    }
}

void Menu::createCircle() {
    double radius = getValidDouble("Введите радиус круга: ");
    figures.push_back(std::make_unique<Circle>(radius));
    std::cout << "Круг успешно добавлен!" << std::endl;
}

void Menu::createRectangle() {
    double width = getValidDouble("Введите ширину прямоугольника: ");
    double height = getValidDouble("Введите высоту прямоугольника: ");
    figures.push_back(std::make_unique<Rectangle>(width, height));
    std::cout << "Прямоугольник успешно добавлен!" << std::endl;
}

void Menu::createTrapezoid() {
    double base1 = getValidDouble("Введите верхнее основание трапеции: ");
    double base2 = getValidDouble("Введите нижнее основание трапеции: ");
    double height = getValidDouble("Введите высоту трапеции: ");
    double side1 = getValidDouble("Введите первую боковую сторону: ");
    double side2 = getValidDouble("Введите вторую боковую сторону: ");
    figures.push_back(std::make_unique<Trapezoid>(base1, base2, height, side1, side2));
    std::cout << "Трапеция успешно добавлена!" << std::endl;
}

void Menu::createTriangle() {
    double side1 = getValidDouble("Введите первую сторону треугольника: ");
    double side2 = getValidDouble("Введите вторую сторону треугольника: ");
    double side3 = getValidDouble("Введите третью сторону треугольника: ");
    figures.push_back(std::make_unique<Triangle>(side1, side2, side3));
    std::cout << "Треугольник успешно добавлен!" << std::endl;
}

void Menu::showAllFigures() const {
    if (figures.empty()) {
        std::cout << "\nНет созданных фигур." << std::endl;
        return;
    }
    
    std::cout << "\n=== ВСЕ ФИГУРЫ ===" << std::endl;
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Фигура #" << (i + 1) << " (" << figures[i]->getType() << ")" << std::endl;
        figures[i]->printInfo();
    }
}

void Menu::showFigure() {
    if (figures.empty()) {
        std::cout << "\nНет созданных фигур." << std::endl;
        return;
    }
    
    int index = getValidInt("Введите номер фигуры (1-" + std::to_string(figures.size()) + "): ");
    
    if (index < 1 || index > static_cast<int>(figures.size())) {
        std::cout << "Неверный номер фигуры!" << std::endl;
        return;
    }
    
    std::cout << "\nФигура #" << index << ":" << std::endl;
    figures[index - 1]->printInfo();
}

void Menu::deleteFigure() {
    if (figures.empty()) {
        std::cout << "\nНет созданных фигур для удаления." << std::endl;
        return;
    }
    
    int index = getValidInt("Введите номер фигуры для удаления (1-" + std::to_string(figures.size()) + "): ");
    
    if (index < 1 || index > static_cast<int>(figures.size())) {
        std::cout << "Неверный номер фигуры!" << std::endl;
        return;
    }
    
    figures.erase(figures.begin() + index - 1);
    std::cout << "Фигура #" << index << " удалена!" << std::endl;
}

void Menu::saveToFile() const {
    if (figures.empty()) {
        std::cout << "\nНет фигур для сохранения." << std::endl;
        return;
    }
    
    std::string filename = getFileName("Введите имя файла для сохранения: ");
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для записи!" << std::endl;
        return;
    }
    
    file << figures.size() << std::endl;
    for (const auto& figure : figures) {
        figure->saveToFile(file);
    }
    
    file.close();
    std::cout << "Фигуры успешно сохранены в файл: " << filename << std::endl;
}

void Menu::loadFromFile() {
    std::cout << "\nФайл должен иметь вид:\n1.Колличество фигур\n2.Название фигуры (Circle/Rectangle/Trapezoid/Trinagle) Линейные размеры, через пробел\n\n";
    std::string filename = getFileName("Введите имя файла для загрузки: ");
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для чтения!" << std::endl;
        return;
    }
    
    int count;
    file >> count;
    
    if (count <= 0) {
        std::cout << "Файл не содержит фигур!" << std::endl;
        file.close();
        return;
    }
    
    std::vector<std::unique_ptr<Figure>> newFigures;
    
    try {
        for (int i = 0; i < count; ++i) {
            std::string type;
            file >> type;
            
            if (type == "Circle") {
                auto circle = std::make_unique<Circle>();
                circle->loadFromFile(file);
                newFigures.push_back(std::move(circle));
            } else if (type == "Rectangle") {
                auto rectangle = std::make_unique<Rectangle>();
                rectangle->loadFromFile(file);
                newFigures.push_back(std::move(rectangle));
            } else if (type == "Trapezoid") {
                auto trapezoid = std::make_unique<Trapezoid>();
                trapezoid->loadFromFile(file);
                newFigures.push_back(std::move(trapezoid));
            } else if (type == "Triangle") {
                auto triangle = std::make_unique<Triangle>();
                triangle->loadFromFile(file);
                newFigures.push_back(std::move(triangle));
            } else {
                std::cout << "Неизвестный тип фигуры: " << type << std::endl;
                file.close();
                return;
            }
        }
        
        std::cout << "Заменить текущие фигуры? (y/n): ";
        std::string answer;
        std::cin >> answer;
        
        if (answer == "y" || answer == "Y" || answer == "да") {
            figures = std::move(newFigures);
            std::cout << "Фигуры успешно загружены из файла: " << filename << std::endl;
            std::cout << "Загружено фигур: " << figures.size() << std::endl;
        } else {
            for (auto& figure : newFigures) {
                figures.push_back(std::move(figure));
            }
            std::cout << "Фигуры добавлены к существующим." << std::endl;
            std::cout << "Общее количество фигур: " << figures.size() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка при загрузке фигур: " << e.what() << std::endl;
    }
    
    file.close();
}

void Menu::saveResultsToFile() const {
    if (figures.empty()) {
        std::cout << "\nНет фигур для сохранения результатов." << std::endl;
        return;
    }
    
    std::string filename = getFileName("Введите имя файла для сохранения результатов: ");
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла для записи!" << std::endl;
        return;
    }
    
    file << "=== РЕЗУЛЬТАТЫ ВЫЧИСЛЕНИЙ ===" << std::endl;
    file << "=============================" << std::endl << std::endl;
    
    double totalArea = 0.0;
    double totalPerimeter = 0.0;
    
    for (size_t i = 0; i < figures.size(); ++i) {
        file << "Фигура #" << (i + 1) << " (" << figures[i]->getType() << ")" << std::endl;
        file << "Площадь: " << std::fixed << std::setprecision(2) << figures[i]->getArea() << std::endl;
        file << "Периметр: " << std::fixed << std::setprecision(2) << figures[i]->getPerimeter() << std::endl;
        file << std::endl;
        
        totalArea += figures[i]->getArea();
        totalPerimeter += figures[i]->getPerimeter();
    }
    
    file << "=== СТАТИСТИКА ===" << std::endl;
    file << "Общее количество фигур: " << figures.size() << std::endl;
    file << "Общая площадь: " << std::fixed << std::setprecision(2) << totalArea << std::endl;
    file << "Общий периметр: " << std::fixed << std::setprecision(2) << totalPerimeter << std::endl;
    file << "Средняя площадь: " << std::fixed << std::setprecision(2) << (totalArea / figures.size()) << std::endl;
    file << "Средний периметр: " << std::fixed << std::setprecision(2) << (totalPerimeter / figures.size()) << std::endl;
    
    file.close();
    std::cout << "Результаты успешно сохранены в файл: " << filename << std::endl;
}

double Menu::getValidDouble(const std::string& prompt) const {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            return value;
        } else {
            std::cout << "Ошибка! Введите положительное число." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int Menu::getValidInt(const std::string& prompt) const {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        } else {
            std::cout << "Ошибка! Введите целое число." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string Menu::getFileName(const std::string& prompt) const {
    std::string filename;
    std::cout << prompt;
    std::cin >> filename;
    return filename;
}

void Menu::run() {
    int choice;
    
    
    do {
        showMainMenu();
        choice = getValidInt("Выберите действие: ");
        
        switch (choice) {
            case 1: addFigure(); break;
            case 2: showAllFigures(); break;
            case 3: showFigure(); break;
            case 4: deleteFigure(); break;
            case 5: saveToFile(); break;
            case 6: loadFromFile(); break;
            case 7: saveResultsToFile(); break;
            case 8:
                figures.clear();
                std::cout << "Все фигуры удалены!" << std::endl;
                break;
            case 9: {
                if (figures.empty()) {
                    std::cout << "\nНет фигур для анализа." << std::endl;
                } else {
                    double totalArea = 0.0, totalPerimeter = 0.0;
                    for (const auto& figure : figures) {
                        totalArea += figure->getArea();
                        totalPerimeter += figure->getPerimeter();
                    }
                    std::cout << "\n=== СТАТИСТИКА ===" << std::endl;
                    std::cout << "Общее количество фигур: " << figures.size() << std::endl;
                    std::cout << "Общая площадь: " << std::fixed << std::setprecision(2) << totalArea << std::endl;
                    std::cout << "Общий периметр: " << std::fixed << std::setprecision(2) << totalPerimeter << std::endl;
                    std::cout << "Средняя площадь: " << std::fixed << std::setprecision(2) << (totalArea / figures.size()) << std::endl;
                    std::cout << "Средний периметр: " << std::fixed << std::setprecision(2) << (totalPerimeter / figures.size()) << std::endl;
                }
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
        }
        
        if (choice != 0) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (choice != 0);
}
