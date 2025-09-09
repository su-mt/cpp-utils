#include "include/queue.h"
#include "include/queue1_public.h"
#include "include/queue1_protected.h"
#include "include/queue1_private.h"
#include <iostream>
#include <limits>

using std::cout;
using std::cin;
using std::endl;
using std::exception;

void clearInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

int safeInputInt() {
    int value;
    cout << "Введите число (от " << INT_MIN << " до " << INT_MAX << "): ";
    cin >> value;
    if (cin.fail()) {
        clearInput();
        throw std::runtime_error("Неверный формат числа или число слишком большое!");
    }
    return value;
}

void testPublicInheritance() {
    QueuePublic queue, copyQueue, mergedQueue;
    bool copyExists = false;
    int choice;
    do {
        cout << "\n--- Меню для Public наследования ---" << endl;
        cout << "1 - Добавить" << endl;
        cout << "2 - Извлечь" << endl;
        cout << "3 - Показать" << endl;
        cout << "4 - Сумма < среднего гармонического" << endl;
        cout << "5 - Копия очереди" << endl;
        cout << "6 - Слияние с копией" << endl;
        cout << "7 - Назад" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        if (cin.fail()) { clearInput(); cout << "Ошибка ввода!" << endl; continue; }
        try {
            switch (choice) {
                case 1: { 
                    try {
                        int value = safeInputInt();
                        queue.add(value);
                        cout << "Число добавлено!" << endl;
                    } catch (const exception& e) {
                        cout << "Ошибка: " << e.what() << endl;
                    }
                    break; 
                }
                case 2: { int value = queue.take(); cout << "Извлечено: " << value << endl; break; }
                case 3: queue.show(); break;
                case 4: cout << "Сумма: " << queue.calculateSum() << endl; break;
                case 5: copyQueue = queue.copy(); copyExists = true; cout << "Копия создана." << endl; break;
                case 6: if (copyExists) { mergedQueue = QueuePublic::merge(queue, copyQueue); cout << "Слияние:" << endl; mergedQueue.show(); } else { cout << "Сначала копия!" << endl; } break;
                case 7: break;
                default: cout << "Ошибка!" << endl; break;
            }
        } catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
    } while (choice != 7);
}

void testProtectedInheritance() {
    QueueProtected queue, copyQueue, mergedQueue;
    bool copyExists = false;
    int choice;
    do {
        cout << "\n--- Меню для Protected наследования ---" << endl;
        cout << "1 - Добавить" << endl;
        cout << "2 - Извлечь" << endl;
        cout << "3 - Показать" << endl;
        cout << "4 - Сумма элементов, меньших среднего гармонического" << endl;
        cout << "5 - Копия очереди" << endl;
        cout << "6 - Слияние с копией" << endl;
        cout << "7 - Назад" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        if (cin.fail()) { clearInput(); cout << "Ошибка ввода!" << endl; continue; }
        try {
            switch (choice) {
                case 1: { 
                    try {
                        int value = safeInputInt();
                        queue.add(value);
                        cout << "Число добавлено!" << endl;
                    } catch (const exception& e) {
                        cout << "Ошибка: " << e.what() << endl;
                    }
                    break; 
                }
                case 2: { int value = queue.take(); cout << "Извлечено: " << value << endl; break; }
                case 3: queue.show(); break;
                case 4: cout << "Сумма: " << queue.calculateSum() << endl; break;
                case 5: copyQueue = queue.copy(); copyExists = true; cout << "Копия создана." << endl; break;
                case 6: if (copyExists) { mergedQueue = QueueProtected::merge(queue, copyQueue); cout << "Слияние:" << endl; mergedQueue.show(); } else { cout << "Сначала копия!" << endl; } break;
                case 7: break;
                default: cout << "Ошибка!" << endl; break;
            }
        } catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
    } while (choice != 7);
}

void testPrivateInheritance() {
    QueuePrivate queue, copyQueue, mergedQueue;
    bool copyExists = false;
    int choice;
    do {
        cout << "\n--- Меню для Private наследования ---" << endl;
        cout << "1 - Добавить" << endl;
        cout << "2 - Извлечь" << endl;
        cout << "3 - Показать" << endl;
        cout << "4 - Сумма < среднего гармонического" << endl;
        cout << "5 - Копия очереди" << endl;
        cout << "6 - Слияние с копией" << endl;
        cout << "7 - Назад" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        if (cin.fail()) { clearInput(); cout << "Ошибка ввода!" << endl; continue; }
        try {
            switch (choice) {
                case 1: { int value; cout << "Введите значение: "; cin >> value; if (cin.fail()) { clearInput(); cout << "Ошибка!" << endl; break; } queue.add(value); break; }
                case 2: { int value = queue.take(); cout << "Извлечено: " << value << endl; break; }
                case 3: queue.show(); break;
                case 4: cout << "Сумма: " << queue.calculateSum() << endl; break;
                case 5: copyQueue = queue.copy(); copyExists = true; cout << "Копия создана." << endl; break;
                case 6: if (copyExists) { mergedQueue = QueuePrivate::merge(queue, copyQueue); cout << "Слияние:" << endl; mergedQueue.show(); } else { cout << "Сначала копия!" << endl; } break;
                case 7: break;
                default: cout << "Ошибка!" << endl; break;
            }
        } catch (const exception& e) { cout << "Ошибка: " << e.what() << endl; }
    } while (choice != 7);
}

int main() {
    cout << "=== СИСТЕМА УПРАВЛЕНИЯ ОЧЕРЕДЯМИ ===" << endl;
    int choice;
    do {
        cout << "\n--- ГЛАВНОЕ МЕНЮ ---" << endl;
        cout << "1 - Public" << endl;
        cout << "2 - Protected" << endl;
        cout << "3 - Private" << endl;
        cout << "4 - Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        if (cin.fail()) { clearInput(); cout << "Ошибка ввода!" << endl; continue; }
        switch (choice) {
            case 1: testPublicInheritance(); break;
            case 2: testProtectedInheritance(); break;
            case 3: testPrivateInheritance(); break;
            case 4: cout << "Завершение программы." << endl; break;
            default: cout << "Ошибка!" << endl; break;
        }
    } while (choice != 4);
    return 0;
}