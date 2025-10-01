#include "include/Keeper.h"
#include <iostream>
#include <limits>

void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    keeper.addBook(book);
    std::cout << "Книга успешно добавлена!" << std::endl;
}

void addTextbookInteractive(Keeper& keeper) {
    std::cout << "\n=== ДОБАВЛЕНИЕ УЧЕБНИКА ===" << std::endl;
    Textbook textbook;
    std::cin >> textbook;
    keeper.addTextbook(textbook);
    std::cout << "Учебник успешно добавлен!" << std::endl;
}

void addStationeryInteractive(Keeper& keeper) {
    std::cout << "\n=== ДОБАВЛЕНИЕ КАНЦЕЛЯРИИ ===" << std::endl;
    Stationery stationery;
    std::cin >> stationery;
    keeper.addStationery(stationery);
    std::cout << "Канцелярия успешно добавлена!" << std::endl;
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
    std::cout << "Введите имя файла для сохранения: ";
    std::string filename;
    std::cin >> filename;
    keeper.saveToFile(filename);
}

void loadFromFileInteractive(Keeper& keeper) {
    std::cout << "Введите имя файла для загрузки: ";
    std::string filename;
    std::cin >> filename;
    keeper.loadFromFile(filename);
}

void showContainerInfo(const Keeper& keeper) {
    std::cout << "\n=== ИНФОРМАЦИЯ О КОНТЕЙНЕРЕ ===" << std::endl;
    std::cout << "Количество товаров: " << keeper.getSize() << std::endl;
    std::cout << "Статус: " << (keeper.isEmpty() ? "пустой" : "содержит данные") << std::endl;
    
    // Подсчет по типам
    auto books = keeper.findByType("BOOK");
    auto textbooks = keeper.findByType("TEXTBOOK");
    auto stationery = keeper.findByType("STATIONERY");
    
    std::cout << "Книг: " << books.size() << std::endl;
    std::cout << "Учебников: " << textbooks.size() << std::endl;
    std::cout << "Канцелярии: " << stationery.size() << std::endl;
}

void demonstrateFeatures() {
    std::cout << "=== ДЕМОНСТРАЦИЯ ВОЗМОЖНОСТЕЙ СИСТЕМЫ ===" << std::endl;
    
    Keeper keeper;
    
    // Добавляем тестовые данные
    std::cout << "\nДобавляем тестовые данные..." << std::endl;
    
    Book book1("Война и мир", "Л.Н. Толстой", 1869, 
               "Роман-эпопея о войне 1812 года", "Классика", 1300, 850.0);
    keeper.addBook(book1);
    
    Textbook textbook1("Математический анализ", "В.А. Зорич", 2019, 
                       "МГУ им. М.В. Ломоносова", 1, 680, 1200.0);
    keeper.addTextbook(textbook1);
    
    Stationery stationery1("Ручка", "Синий", "Письмо", 25.0);
    keeper.addStationery(stationery1);
    
    std::cout << "Тестовые данные добавлены." << std::endl;
    
    // Показываем все товары
    std::cout << "\nВсе товары в магазине:" << std::endl;
    keeper.displayAll();
    
    // Сохраняем в файл
    std::cout << "\nСохраняем данные в файл 'test_data.txt'..." << std::endl;
    keeper.saveToFile("test_data.txt");
    
    // Очищаем контейнер
    std::cout << "\nОчищаем контейнер..." << std::endl;
    keeper.removeAllItems();
    
    // Загружаем из файла
    std::cout << "\nЗагружаем данные из файла..." << std::endl;
    keeper.loadFromFile("test_data.txt");
    
    // Показываем загруженные данные
    std::cout << "\nЗагруженные данные:" << std::endl;
    keeper.displayAll();
    
    std::cout << "\nДемонстрация завершена. Переходим к интерактивному режиму." << std::endl;
}

int main() {
    std::cout << "Добро пожаловать в систему управления книжным магазином!" << std::endl;
    
    char choice;
    std::cout << "Запустить демонстрацию? (y/n): ";
    std::cin >> choice;
    clearInputStream();
    
    Keeper keeper;
    
    if (choice == 'y' || choice == 'Y') {
        demonstrateFeatures();
    }
    
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
                std::cout << "Спасибо за использование системы!" << std::endl;
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
        }
        
        if (menuChoice != 0) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
        
    } while (menuChoice != 0);
    
    return 0;
}