#include "include/Keeper.h"
#include "include/json.hpp"
#include <iostream>
#include <algorithm>

using json = nlohmann::json;


Keeper::Keeper() {}


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
    items.erase_at(index);
    std::cout << "Элемент с индексом " << index << " удален." << std::endl;
    return true;
}

void Keeper::removeAllItems() {
    items.clear();
    std::cout << "Все элементы удалены из контейнера." << std::endl;
}


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


bool Keeper::saveToFile(const std::string& filename) const {
    json j = json::array();
    
    for (const auto& item : items) {
        json obj;
        std::string type = item->getType();
        obj["type"] = type;
        
        if (type == "BOOK") {
            const Book* book = dynamic_cast<const Book*>(item.get());
            obj["title"] = book->getTitle();
            obj["author"] = book->getAuthor();
            obj["year"] = book->getYear();
            obj["annotation"] = book->getAnnotation();
            obj["genre"] = book->getGenre();
            obj["pages"] = book->getPages();
            obj["cost"] = book->getCost();
        }
        else if (type == "TEXTBOOK") {
            const Textbook* textbook = dynamic_cast<const Textbook*>(item.get());
            obj["title"] = textbook->getTitle();
            obj["author"] = textbook->getAuthor();
            obj["year"] = textbook->getYear();
            obj["institution"] = textbook->getInstitution();
            obj["studyYear"] = textbook->getStudyYear();
            obj["pages"] = textbook->getPages();
            obj["cost"] = textbook->getCost();
        }
        else if (type == "STATIONERY") {
            const Stationery* stationery = dynamic_cast<const Stationery*>(item.get());
            obj["stationeryType"] = stationery->getStationeryType();
            obj["color"] = stationery->getColor();
            obj["purpose"] = stationery->getPurpose();
            obj["cost"] = stationery->getCost();
        }
        
        j.push_back(obj);
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << " для записи." << std::endl;
        return false;
    }
    
    file << j.dump(4); 
    file.close();
    std::cout << "Данные успешно сохранены в файл " << filename << " в формате JSON." << std::endl;
    return true;
}

bool Keeper::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << filename << " для чтения." << std::endl;
        return false;
    }
    
    items.clear(); 
    
    json j;
    try {
        file >> j;
    } catch (json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
        file.close();
        return false;
    }
    
    for (const auto& obj : j) {
        std::string type = obj["type"];
        
        if (type == "BOOK") {
            auto book = std::make_unique<Book>();
            book->setTitle(obj["title"]);
            book->setAuthor(obj["author"]);
            book->setYear(obj["year"]);
            book->setAnnotation(obj["annotation"]);
            book->setGenre(obj["genre"]);
            book->setPages(obj["pages"]);
            book->setCost(obj["cost"]);
            items.push_back(std::move(book));
        }
        else if (type == "TEXTBOOK") {
            auto textbook = std::make_unique<Textbook>();
            textbook->setTitle(obj["title"]);
            textbook->setAuthor(obj["author"]);
            textbook->setYear(obj["year"]);
            textbook->setInstitution(obj["institution"]);
            textbook->setStudyYear(obj["studyYear"]);
            textbook->setPages(obj["pages"]);
            textbook->setCost(obj["cost"]);
            items.push_back(std::move(textbook));
        }
        else if (type == "STATIONERY") {
            auto stationery = std::make_unique<Stationery>();
            stationery->setStationeryType(obj["stationeryType"]);
            stationery->setColor(obj["color"]);
            stationery->setPurpose(obj["purpose"]);
            stationery->setCost(obj["cost"]);
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

my_vector<size_t> Keeper::findByType(const std::string& type) const {
    my_vector<size_t> indices;
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getType() == type) {
            indices.push_back(i);
        }
    }
    return indices;
}


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


std::ostream& operator<<(std::ostream& os, const Keeper& keeper) {
    keeper.displayAll();
    return os;
}