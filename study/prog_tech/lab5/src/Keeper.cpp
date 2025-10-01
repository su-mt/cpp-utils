#include "include/Keeper.h"
#include <iostream>
#include <algorithm>

// Конструктор
Keeper::Keeper() {}

// Методы для работы с объектами
void Keeper::addItem(std::unique_ptr<Base> item) {
    if (item) {
        items.push_back(std::move(item));
    }
}

void Keeper::addBook(const Book& book) {
    items.push_back(std::make_unique<Book>(book));
}

void Keeper::addTextbook(const Textbook& textbook) {
    items.push_back(std::make_unique<Textbook>(textbook));
}

void Keeper::addStationery(const Stationery& stationery) {
    items.push_back(std::make_unique<Stationery>(stationery));
}

bool Keeper::removeItem(size_t index) {
    if (index >= items.size()) {
        std::cout << "Ошибка: неверный индекс " << index << std::endl;
        return false;
    }
    items.erase(items.begin() + index);
    std::cout << "Элемент с индексом " << index << " удален." << std::endl;
    return true;
}

void Keeper::removeAllItems() {
    items.clear();
    std::cout << "Все элементы удалены из контейнера." << std::endl;
}

// Методы для отображения
void Keeper::displayAll() const {
    if (items.empty()) {
        std::cout << "Контейнер пуст." << std::endl;
        return;
    }
    
    std::cout << "=== СОДЕРЖИМОЕ КНИЖНОГО МАГАЗИНА ===" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << "Индекс [" << i << "]:" << std::endl;
        items[i]->display();
        std::cout << std::endl;
    }
}

void Keeper::displayItem(size_t index) const {
    if (index >= items.size()) {
        std::cout << "Ошибка: неверный индекс " << index << std::endl;
        return;
    }
    std::cout << "Индекс [" << index << "]:" << std::endl;
    items[index]->display();
}

size_t Keeper::getSize() const {
    return items.size();
}

bool Keeper::isEmpty() const {
    return items.empty();
}

// Методы для работы с файлами
bool Keeper::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        return false;
    }
    
    file << items.size() << std::endl; // записываем количество элементов
    
    for (const auto& item : items) {
        item->saveToFile(file);
    }
    
    file.close();
    std::cout << "Данные успешно сохранены в файл " << filename << std::endl;
    return true;
}

bool Keeper::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << " для чтения." << std::endl;
        return false;
    }
    
    items.clear(); // очищаем текущие данные
    
    size_t count;
    file >> count;
    file.ignore(); // игнорируем символ новой строки
    
    for (size_t i = 0; i < count; ++i) {
        std::string type;
        std::getline(file, type);
        
        if (type == "BOOK") {
            auto book = std::make_unique<Book>();
            book->loadFromFile(file);
            items.push_back(std::move(book));
        }
        else if (type == "TEXTBOOK") {
            auto textbook = std::make_unique<Textbook>();
            textbook->loadFromFile(file);
            items.push_back(std::move(textbook));
        }
        else if (type == "STATIONERY") {
            auto stationery = std::make_unique<Stationery>();
            stationery->loadFromFile(file);
            items.push_back(std::move(stationery));
        }
        else {
            std::cout << "Ошибка: неизвестный тип объекта " << type << std::endl;
            file.close();
            return false;
        }
    }
    
    file.close();
    std::cout << "Данные успешно загружены из файла " << filename << std::endl;
    std::cout << "Загружено " << items.size() << " элементов." << std::endl;
    return true;
}

// Поиск и фильтрация
void Keeper::displayByType(const std::string& type) const {
    bool found = false;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getType() == type) {
            if (!found) {
                std::cout << "=== " << type << " ===" << std::endl;
                found = true;
            }
            std::cout << "Индекс [" << i << "]:" << std::endl;
            items[i]->display();
            std::cout << std::endl;
        }
    }
    
    if (!found) {
        std::cout << "Объекты типа " << type << " не найдены." << std::endl;
    }
}

std::vector<size_t> Keeper::findByType(const std::string& type) const {
    std::vector<size_t> indices;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getType() == type) {
            indices.push_back(i);
        }
    }
    return indices;
}

// Операторы доступа
Base* Keeper::operator[](size_t index) {
    if (index >= items.size()) {
        return nullptr;
    }
    return items[index].get();
}

const Base* Keeper::operator[](size_t index) const {
    if (index >= items.size()) {
        return nullptr;
    }
    return items[index].get();
}

// Операторы ввода-вывода
std::ostream& operator<<(std::ostream& os, const Keeper& keeper) {
    keeper.displayAll();
    return os;
}