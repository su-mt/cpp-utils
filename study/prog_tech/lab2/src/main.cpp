#include "include/queue.hpp"
#include "include/long_number.hpp"
#include <iostream>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMainMenu() {
    std::cout << "\n=== МЕНЮ ТЕСТИРОВАНИЯ ===" << std::endl;
    std::cout << "1. Тестирование класса Queue (очередь)" << std::endl;
    std::cout << "2. Тестирование класса LongNumber" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}

void printQueueMenu() {
    std::cout << "\n=== МЕНЮ ОЧЕРЕДИ ===" << std::endl;
    std::cout << "1. Создать пустую очередь" << std::endl;
    std::cout << "2. Инициализировать нулями" << std::endl;
    std::cout << "3. Инициализировать случайными числами" << std::endl;
    std::cout << "4. Добавить элемент" << std::endl;
    std::cout << "5. Извлечь элемент" << std::endl;
    std::cout << "6. Показать очередь" << std::endl;
    std::cout << "7. Размер очереди" << std::endl;
    std::cout << "8. Очистить очередь" << std::endl;
    std::cout << "9. Установить пользовательский шаг" << std::endl;
    std::cout << "10. Тест префиксных операторов (++ --)" << std::endl;
    std::cout << "11. Тест постфиксных операторов (++ --)" << std::endl;
    std::cout << "0. Назад в главное меню" << std::endl;
    std::cout << "Выберите опцию: ";
}

void printLongNumberMenu() {
    std::cout << "\n=== МЕНЮ LONGNUMBER ===" << std::endl;
    std::cout << "1. Интерактивное сравнение двух чисел" << std::endl;
    std::cout << "2. Тестирование операторов равенства (== !=)" << std::endl;
    std::cout << "3. Тестирование операторов порядка (< >)" << std::endl;
    std::cout << "4. Тестирование операторов порядка с равенством (<= >=)" << std::endl;
    std::cout << "5. Демонстрация всех операторов с одним числом" << std::endl;
    std::cout << "6. Комплексное тестирование с тремя числами" << std::endl;
    std::cout << "0. Назад в главное меню" << std::endl;
    std::cout << "Выберите опцию: ";
}

void testQueue() {
    Queue q;
    int choice;
    
    do {
        printQueueMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            clearInput();
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                q.clear();
                std::cout << "Создана пустая очередь." << std::endl;
                break;
            }
            case 2: {
                int n;
                std::cout << "Введите размер: ";
                std::cin >> n;
                if (std::cin.fail() || n < 0) {
                    clearInput();
                    std::cout << "Неверный размер!" << std::endl;
                    break;
                }
                q.init(n);
                std::cout << "Очередь инициализирована " << n << " нулями." << std::endl;
                break;
            }
            case 3: {
                int n, minVal, maxVal;
                std::cout << "Введите размер: ";
                std::cin >> n;
                std::cout << "Введите минимальное значение: ";
                std::cin >> minVal;
                std::cout << "Введите максимальное значение: ";
                std::cin >> maxVal;
                if (std::cin.fail() || n < 0 || minVal > maxVal) {
                    clearInput();
                    std::cout << "Неверные параметры!" << std::endl;
                    break;
                }
                q.initRandom(n, minVal, maxVal);
                std::cout << "Очередь инициализирована случайными числами." << std::endl;
                break;
            }
            case 4: {
                int value;
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверное значение!" << std::endl;
                    break;
                }
                q.add(value);
                std::cout << "Значение " << value << " добавлено." << std::endl;
                break;
            }
            case 5: {
                try {
                    int value = q.take();
                    std::cout << "Извлечено значение: " << value << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка: " << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                q.show();
                break;
            }
            case 7: {
                std::cout << "Размер очереди: " << q.size() << std::endl;
                break;
            }
            case 8: {
                q.clear();
                std::cout << "Очередь очищена." << std::endl;
                break;
            }
            case 9: {
                int step;
                std::cout << "Введите новый шаг (текущий: " << q.getUserStep() << "): ";
                std::cin >> step;
                if (std::cin.fail() || step < 0) {
                    clearInput();
                    std::cout << "Неверный шаг!" << std::endl;
                    break;
                }
                q.setUserStep(step);
                std::cout << "Установлен шаг: " << step << std::endl;
                break;
            }
            case 10: {
                std::cout << "Текущая очередь: ";
                q.show();
                std::cout << "Применяем ++q (добавляет " << q.getUserStep() << " элементов): ";
                ++q;
                q.show();
                std::cout << "Применяем --q (удаляет " << q.getUserStep() << " элементов): ";
                --q;
                q.show();
                break;
            }
            case 11: {
                if (q.empty()) {
                    std::cout << "Очередь пуста! Добавьте элементы сначала." << std::endl;
                    break;
                }
                std::cout << "Текущая очередь: ";
                q.show();
                
                std::cout << "Применяем q++ (увеличивает все на минимальное): ";
                Queue old = q++;
                std::cout << "\nСтарая: ";
                old.show();
                std::cout << "Новая: ";
                q.show();
                
                std::cout << "Применяем q-- (уменьшает все на максимальное): ";
                Queue old2 = q--;
                std::cout << "\nСтарая: ";
                old2.show();
                std::cout << "Новая: ";
                q.show();
                break;
            }
            case 0: {
                std::cout << "Возврат в главное меню..." << std::endl;
                break;
            }
            default: {
                std::cout << "Неверная опция!" << std::endl;
                break;
            }
        }
    } while (choice != 0);
}

void testLongNumber() {
    int choice;
    
    do {
        printLongNumberMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            clearInput();
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
            continue;
        }
        
        switch (choice) {
            case 1: {
                long val1, val2;
                std::cout << "Введите первое число: ";
                std::cin >> val1;
                std::cout << "Введите второе число: ";
                std::cin >> val2;
                
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                
                LongNumber a(val1);
                LongNumber b(val2);
                
                std::cout << "\n--- Результаты сравнения ---" << std::endl;
                std::cout << "a = "; a.print(); 
                std::cout << ", b = "; b.print(); std::cout << std::endl;
                
                std::cout << "a == b: " << (a == b ? "true" : "false") << std::endl;
                std::cout << "a != b: " << (a != b ? "true" : "false") << std::endl;
                std::cout << "a < b:  " << (a < b ? "true" : "false") << std::endl;
                std::cout << "a > b:  " << (a > b ? "true" : "false") << std::endl;
                std::cout << "a <= b: " << (a <= b ? "true" : "false") << std::endl;
                std::cout << "a >= b: " << (a >= b ? "true" : "false") << std::endl;
                break;
            }
            case 2: {
                long val;
                std::cout << "Введите число для тестирования: ";
                std::cin >> val;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                LongNumber a(val);
                long testVal = val + 10;
                
                std::cout << "\n--- Тестирование операторов == и != ---" << std::endl;
                std::cout << "Число a = " << val << std::endl;
                std::cout << "Тестовое значение = " << testVal << std::endl;
                
                std::cout << "\nМетоды класса:" << std::endl;
                std::cout << "a == " << val << ": " << (a == val ? "true" : "false") << std::endl;
                std::cout << "a == " << testVal << ": " << (a == testVal ? "true" : "false") << std::endl;
                std::cout << "a != " << val << ": " << (a != val ? "true" : "false") << std::endl;
                std::cout << "a != " << testVal << ": " << (a != testVal ? "true" : "false") << std::endl;
                
                std::cout << "\nДружественные функции:" << std::endl;
                std::cout << val << " == a: " << (val == a ? "true" : "false") << std::endl;
                std::cout << testVal << " == a: " << (testVal == a ? "true" : "false") << std::endl;
                std::cout << val << " != a: " << (val != a ? "true" : "false") << std::endl;
                std::cout << testVal << " != a: " << (testVal != a ? "true" : "false") << std::endl;
                break;
            }
            case 3: {
                long val1, val2;
                std::cout << "Введите первое число: ";
                std::cin >> val1;
                std::cout << "Введите второе число: ";
                std::cin >> val2;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                LongNumber a(val1);
                LongNumber b(val2);
                
                std::cout << "\n--- Тестирование операторов < и > ---" << std::endl;
                std::cout << "a = " << val1 << ", b = " << val2 << std::endl;
                
                std::cout << "\nМетоды класса:" << std::endl;
                std::cout << "a < b: " << (a < b ? "true" : "false") << std::endl;
                std::cout << "a < " << val2 << ": " << (a < val2 ? "true" : "false") << std::endl;
                
                std::cout << "\nДружественные функции:" << std::endl;
                std::cout << "a > b: " << (a > b ? "true" : "false") << std::endl;
                std::cout << "b > a: " << (b > a ? "true" : "false") << std::endl;
                std::cout << "a > " << val2 << ": " << (a > val2 ? "true" : "false") << std::endl;
                std::cout << val1 << " > b: " << (val1 > b ? "true" : "false") << std::endl;
                std::cout << val2 << " < a: " << (val2 < a ? "true" : "false") << std::endl;
                break;
            }
            case 4: {
                long val1, val2;
                std::cout << "Введите первое число: ";
                std::cin >> val1;
                std::cout << "Введите второе число: ";
                std::cin >> val2;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                LongNumber a(val1);
                LongNumber b(val2);
                
                std::cout << "\n--- Тестирование операторов <= и >= ---" << std::endl;
                std::cout << "a = " << val1 << ", b = " << val2 << std::endl;
                
                std::cout << "\nДружественные функции:" << std::endl;
                std::cout << "a <= b: " << (a <= b ? "true" : "false") << std::endl;
                std::cout << "b <= a: " << (b <= a ? "true" : "false") << std::endl;
                std::cout << "a >= b: " << (a >= b ? "true" : "false") << std::endl;
                std::cout << "b >= a: " << (b >= a ? "true" : "false") << std::endl;
                
                std::cout << "\nСравнение с числами:" << std::endl;
                std::cout << "a <= " << val2 << ": " << (a <= val2 ? "true" : "false") << std::endl;
                std::cout << val1 << " <= b: " << (val1 <= b ? "true" : "false") << std::endl;
                std::cout << "a >= " << val2 << ": " << (a >= val2 ? "true" : "false") << std::endl;
                std::cout << val1 << " >= b: " << (val1 >= b ? "true" : "false") << std::endl;
                break;
            }
            case 5: {
                long val;
                std::cout << "Введите число: ";
                std::cin >> val;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                LongNumber a(val);
                long smaller = val - 5;
                long larger = val + 5;
                
                std::cout << "\n--- Демонстрация всех операторов ---" << std::endl;
                std::cout << "Число a = " << val << std::endl;
                std::cout << "Меньшее число = " << smaller << std::endl;
                std::cout << "Большее число = " << larger << std::endl;
                
                std::cout << "\nОператоры равенства (методы):" << std::endl;
                std::cout << "a == " << val << ": " << (a == val ? "true" : "false") << std::endl;
                std::cout << "a != " << smaller << ": " << (a != smaller ? "true" : "false") << std::endl;
                
                std::cout << "\nОператоры порядка (методы):" << std::endl;
                std::cout << "a < " << larger << ": " << (a < larger ? "true" : "false") << std::endl;
                
                std::cout << "\nОператоры (дружественные функции):" << std::endl;
                std::cout << "a > " << smaller << ": " << (a > smaller ? "true" : "false") << std::endl;
                std::cout << "a <= " << val << ": " << (a <= val ? "true" : "false") << std::endl;
                std::cout << "a >= " << smaller << ": " << (a >= smaller ? "true" : "false") << std::endl;
                
                std::cout << "\nСимметричные операторы:" << std::endl;
                std::cout << val << " == a: " << (val == a ? "true" : "false") << std::endl;
                std::cout << larger << " > a: " << (larger > a ? "true" : "false") << std::endl;
                std::cout << smaller << " < a: " << (smaller < a ? "true" : "false") << std::endl;
                break;
            }
            case 6: {
                long val1, val2, val3;
                std::cout << "Введите три числа через пробел: ";
                std::cin >> val1 >> val2 >> val3;
                if (std::cin.fail()) {
                    clearInput();
                    std::cout << "Неверный ввод!" << std::endl;
                    break;
                }
                LongNumber a(val1), b(val2), c(val3);
                
                std::cout << "\n--- Комплексное тестирование ---" << std::endl;
                std::cout << "a = " << val1 << ", b = " << val2 << ", c = " << val3 << std::endl;
                
                std::cout << "\nСравнение между объектами:" << std::endl;
                std::cout << "a == b: " << (a == b ? "true" : "false") << std::endl;
                std::cout << "a == c: " << (a == c ? "true" : "false") << std::endl;
                std::cout << "b == c: " << (b == c ? "true" : "false") << std::endl;
                
                std::cout << "\nПорядковые отношения:" << std::endl;
                std::cout << "a < b: " << (a < b ? "true" : "false") << std::endl;
                std::cout << "b > c: " << (b > c ? "true" : "false") << std::endl;
                std::cout << "a <= c: " << (a <= c ? "true" : "false") << std::endl;
                std::cout << "c >= a: " << (c >= a ? "true" : "false") << std::endl;
                
                std::cout << "\nСмешанные операторы (объект-число):" << std::endl;
                std::cout << "a == " << val1 << ": " << (a == val1 ? "true" : "false") << std::endl;
                std::cout << val2 << " == b: " << (val2 == b ? "true" : "false") << std::endl;
                std::cout << "c > " << val1 << ": " << (c > val1 ? "true" : "false") << std::endl;
                std::cout << val3 << " <= a: " << (val3 <= a ? "true" : "false") << std::endl;
                break;
            }
            case 0: {
                std::cout << "Возврат в главное меню..." << std::endl;
                break;
            }
            default: {
                std::cout << "Неверная опция!" << std::endl;
                break;
            }
        }
    } while (choice != 0);
}

int main() {
    int choice;
    
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА 2 ===" << std::endl;
    std::cout << "Тестирование классов Queue и LongNumber" << std::endl;
    
    do {
        printMainMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            clearInput();
            std::cout << "Неверный ввод! Попробуйте снова." << std::endl;
            continue;
        }
        
        switch (choice) {
            case 1:
                testQueue();
                break;
            case 2:
                testLongNumber();
                break;
            case 0:
                std::cout << "Выход из программы. До свидания!" << std::endl;
                break;
            default:
                std::cout << "Неверная опция! Попробуйте снова." << std::endl;
                break;
        }
    } while (choice != 0);
    
    return 0;
}

