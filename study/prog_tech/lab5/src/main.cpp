#include "include/Keeper.h"
#include <iostream>
#include <limits>
#include <chrono>
#include <iomanip>
#include <sstream>

void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


std::string generateFilenameWithTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << "bookstore_" 
        << std::put_time(&tm, "%Y-%m-%d_%H%M%S")
        << ".json";
    return oss.str();
}


void autoSaveKeeper(const Keeper& keeper) {
    std::string filename = generateFilenameWithTimestamp();
    keeper.saveToFile(filename);
}

void showMenu() {
    std::cout << "\n=== КНИЖНЫЙ МАГАЗИН - СИСТЕМА УПРАВЛЕНИЯ ===" << std::endl;
    std::cout << "1. Добавить книгу" << std::endl;
    std::cout << "2. Добавить учебник" << std::endl;
    std::cout << "3. Добавить канцелярию" << std::endl;
    std::cout << "4. Показать все товары" << std::endl;
    std::cout << "5. Показать товары по типу" << std::endl;
    std::cout << "6. Удалить товар по индексу" << std::endl;
    std::cout << "7. Очистить все товары" << std::endl;
    std::cout << "8. Сохранить в файл" << std::endl;
    std::cout << "9. Загрузить из файла" << std::endl;
    std::cout << "10. Показать информацию о контейнере" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите пункт меню: ";
}

void addBookInteractive(Keeper& keeper) {
    std::cout << "\n=== ДОБАВЛЕНИЕ КНИГИ ===" << std::endl;
    Book book;
    std::cin >> book;
    
    
    if (book.getTitle().empty() || book.getAuthor().empty()) {
        std::cout << "ОШИБКА: Название и автор книги не могут быть пустыми!" << std::endl;
        clearInputStream();
        return;
    }
    if (book.getYear() <= 0 || book.getYear() > 2100) {
        std::cout << "ОШИБКА: Год выпуска должен быть между 1 и 2100!" << std::endl;
        clearInputStream();
        return;
    }
    if (book.getPages() <= 0) {
        std::cout << "ОШИБКА: Количество страниц должно быть положительным числом!" << std::endl;
        clearInputStream();
        return;
    }
    if (book.getCost() < 0) {
        std::cout << "ОШИБКА: Стоимость не может быть отрицательной!" << std::endl;
        clearInputStream();
        return;
    }
    
    keeper.addBook(book);
    std::cout << "Книга успешно добавлена!" << std::endl;
    autoSaveKeeper(keeper);
}

void addTextbookInteractive(Keeper& keeper) {
    std::cout << "\n=== ДОБАВЛЕНИЕ УЧЕБНИКА ===" << std::endl;
    Textbook textbook;
    std::cin >> textbook;
    
    
    if (textbook.getTitle().empty() || textbook.getAuthor().empty()) {
        std::cout << "ОШИБКА: Название и автор учебника не могут быть пустыми!" << std::endl;
        clearInputStream();
        return;
    }
    if (textbook.getYear() <= 0 || textbook.getYear() > 2100) {
        std::cout << "ОШИБКА: Год выпуска должен быть между 1 и 2100!" << std::endl;
        clearInputStream();
        return;
    }
    if (textbook.getStudyYear() <= 0 || textbook.getStudyYear() > 8) {
        std::cout << "ОШИБКА: Год обучения должен быть от 1 до 8!" << std::endl;
        clearInputStream();
        return;
    }
    if (textbook.getPages() <= 0) {
        std::cout << "ОШИБКА: Количество страниц должно быть положительным числом!" << std::endl;
        clearInputStream();
        return;
    }
    if (textbook.getCost() < 0) {
        std::cout << "ОШИБКА: Стоимость не может быть отрицательной!" << std::endl;
        clearInputStream();
        return;
    }
    
    keeper.addTextbook(textbook);
    std::cout << "Учебник успешно добавлен!" << std::endl;
    autoSaveKeeper(keeper);
}

void addStationeryInteractive(Keeper& keeper) {
    std::cout << "\n=== ДОБАВЛЕНИЕ КАНЦЕЛЯРИИ ===" << std::endl;
    Stationery stationery;
    std::cin >> stationery;
    
    
    if (stationery.getStationeryType().empty()) {
        std::cout << "ОШИБКА: Тип канцелярии не может быть пустым!" << std::endl;
        clearInputStream();
        return;
    }
    if (stationery.getCost() < 0) {
        std::cout << "ОШИБКА: Стоимость не может быть отрицательной!" << std::endl;
        clearInputStream();
        return;
    }
    
    keeper.addStationery(stationery);
    std::cout << "Канцелярия успешно добавлена!" << std::endl;
    autoSaveKeeper(keeper);
}

void showByType(const Keeper& keeper) {
    std::cout << "\nВыберите тип товара:" << std::endl;
    std::cout << "1. Книги (BOOK)" << std::endl;
    std::cout << "2. Учебники (TEXTBOOK)" << std::endl;
    std::cout << "3. Канцелярия (STATIONERY)" << std::endl;
    std::cout << "Ваш выбор: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            keeper.displayByType("BOOK");
            break;
        case 2:
            keeper.displayByType("TEXTBOOK");
            break;
        case 3:
            keeper.displayByType("STATIONERY");
            break;
        default:
            std::cout << "Неверный выбор!" << std::endl;
    }
}

void removeByIndex(Keeper& keeper) {
    if (keeper.isEmpty()) {
        std::cout << "Контейнер пуст. Нечего удалять." << std::endl;
        return;
    }
    
    std::cout << "Введите индекс товара для удаления (0-" << keeper.getSize() - 1 << "): ";
    size_t index;
    std::cin >> index;
    keeper.removeItem(index);
}

void saveToFileInteractive(const Keeper& keeper) {
    std::cout << "Введите имя файла (или нажмите Enter для автоматического имени с датой): ";
    std::string filename;
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        filename = generateFilenameWithTimestamp();
        std::cout << "Используется автоматическое имя: " << filename << std::endl;
    }
    
    keeper.saveToFile(filename);
}

void loadFromFileInteractive(Keeper& keeper) {
    std::cout << "Введите имя файла для загрузки: ";
    std::string filename;
    std::getline(std::cin, filename);
    
    if (filename.empty()) {
        std::cout << "Ошибка: имя файла не может быть пустым!" << std::endl;
        return;
    }
    
    keeper.loadFromFile(filename);
}

void showContainerInfo(const Keeper& keeper) {
    std::cout << "\n=== ИНФОРМАЦИЯ О КОНТЕЙНЕРЕ ===" << std::endl;
    std::cout << "Количество товаров: " << keeper.getSize() << std::endl;
    std::cout << "Статус: " << (keeper.isEmpty() ? "пустой" : "содержит данные") << std::endl;
    
    
    auto books = keeper.findByType("BOOK");
    auto textbooks = keeper.findByType("TEXTBOOK");
    auto stationery = keeper.findByType("STATIONERY");
    
    std::cout << "Книг: " << books.size() << std::endl;
    std::cout << "Учебников: " << textbooks.size() << std::endl;
    std::cout << "Канцелярии: " << stationery.size() << std::endl;
}



int main() {
    std::cout << "Добро пожаловать в систему управления книжным магазином!" << std::endl;
    
    Keeper keeper;
    
    int menuChoice;
    do {
        showMenu();
        std::cin >> menuChoice;
        clearInputStream();
        
        switch (menuChoice) {
            case 1:
                addBookInteractive(keeper);
                break;
            case 2:
                addTextbookInteractive(keeper);
                break;
            case 3:
                addStationeryInteractive(keeper);
                break;
            case 4:
                keeper.displayAll();
                break;
            case 5:
                showByType(keeper);
                break;
            case 6:
                removeByIndex(keeper);
                break;
            case 7:
                keeper.removeAllItems();
                break;
            case 8:
                saveToFileInteractive(keeper);
                break;
            case 9:
                loadFromFileInteractive(keeper);
                break;
            case 10:
                showContainerInfo(keeper);
                break;
            case 0:
                
                break;
            default:
                std::cout << "Неверный выбор. " << std::endl;
        }
        
        if (menuChoice != 0) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
        
    } while (menuChoice != 0);
    
    return 0;
}