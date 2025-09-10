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
    std::cout << "1. Создать два числа и сравнить" << std::endl;
    std::cout << "2. Тестировать операторы == и !=" << std::endl;
    std::cout << "3. Тестировать операторы < и >" << std::endl;
    std::cout << "4. Тестировать операторы <= и >=" << std::endl;
    std::cout << "5. Полное тестирование всех операторов" << std::endl;
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
                
                std::cout << "a = "; a.print(); 
                std::cout << ", b = "; b.print(); std::cout << std::endl;
                
                std::cout << "a == b: " << (a == b) << std::endl;
                std::cout << "a != b: " << (a != b) << std::endl;
                std::cout << "a < b: " << (a < b) << std::endl;
                std::cout << "a > b: " << (a > b) << std::endl;
                std::cout << "a <= b: " << (a <= b) << std::endl;
                std::cout << "a >= b: " << (a >= b) << std::endl;
                break;
            }
            case 2: {
                LongNumber a(10);
                std::cout << "Тестирование == и != (a = 10)" << std::endl;
                std::cout << "a == 10: " << (a == 10) << std::endl;
                std::cout << "10 == a: " << (10 == a) << std::endl;
                std::cout << "a == 5: " << (a == 5) << std::endl;
                std::cout << "a != 5: " << (a != 5) << std::endl;
                std::cout << "5 != a: " << (5 != a) << std::endl;
                break;
            }
            case 3: {
                LongNumber a(10);
                std::cout << "Тестирование < и > (a = 10)" << std::endl;
                std::cout << "a < 15: " << (a < 15) << std::endl;
                std::cout << "15 < a: " << (15 < a) << std::endl;
                std::cout << "a > 5: " << (a > 5) << std::endl;
                std::cout << "5 > a: " << (5 > a) << std::endl;
                break;
            }
            case 4: {
                LongNumber a(10);
                std::cout << "Тестирование <= и >= (a = 10)" << std::endl;
                std::cout << "a <= 10: " << (a <= 10) << std::endl;
                std::cout << "10 <= a: " << (10 <= a) << std::endl;
                std::cout << "a <= 15: " << (a <= 15) << std::endl;
                std::cout << "a >= 5: " << (a >= 5) << std::endl;
                std::cout << "5 >= a: " << (5 >= a) << std::endl;
                break;
            }
            case 5: {
                LongNumber a(10), b(20), c(10);
                std::cout << "Полное тестирование (a=10, b=20, c=10)" << std::endl;
                
                std::cout << "\nМетоды класса:" << std::endl;
                std::cout << "a == c: " << (a == c) << std::endl;
                std::cout << "a != b: " << (a != b) << std::endl;
                std::cout << "a < b: " << (a < b) << std::endl;
                
                std::cout << "\nДружественные функции:" << std::endl;
                std::cout << "b > a: " << (b > a) << std::endl;
                std::cout << "a <= c: " << (a <= c) << std::endl;
                std::cout << "b >= a: " << (b >= a) << std::endl;
                
                std::cout << "\nСравнение с числами:" << std::endl;
                std::cout << "a == 10: " << (a == 10) << std::endl;
                std::cout << "10 == a: " << (10 == a) << std::endl;
                std::cout << "a > 5: " << (a > 5) << std::endl;
                std::cout << "5 < a: " << (5 < a) << std::endl;
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
