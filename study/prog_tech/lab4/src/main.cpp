#include "include/linear_search.h"
#include "include/binary_tree.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

std::vector<int> current_int_array;
std::vector<char> current_char_array;
std::vector<float> current_float_array;
std::vector<double> current_double_array;
std::vector<char*> current_charptr_array;

void show_main_menu() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      ЛАБОРАТОРНАЯ РАБОТА 4" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "1. Работа с линейным поиском" << std::endl;
    std::cout << "2. Работа с бинарным деревом" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Выберите раздел: ";
}

void show_data_type_menu() {
    std::cout << "\n--- Выбор типа данных ---" << std::endl;
    std::cout << "1. Целые числа (int)" << std::endl;
    std::cout << "2. Символы (char)" << std::endl;
    std::cout << "3. Числа с плавающей точкой (float)" << std::endl;
    std::cout << "4. Числа двойной точности (double)" << std::endl;
    std::cout << "5. Строки (char*)" << std::endl;
    std::cout << "0. Вернуться в главное меню" << std::endl;
    std::cout << "Выберите тип данных: ";
}

void show_linear_search_menu() {
    std::cout << "\n--- Меню линейного поиска ---" << std::endl;
    std::cout << "1. Сгенерировать новый массив" << std::endl;
    std::cout << "2. Показать текущий массив" << std::endl;
    std::cout << "3. Найти элемент в массиве" << std::endl;
    std::cout << "0. Назад к выбору типа данных" << std::endl;
    std::cout << "Выберите действие: ";
}

void show_tree_menu() {
    std::cout << "\n--- Меню бинарного дерева ---" << std::endl;
    std::cout << "1. Целые числа (int)" << std::endl;
    std::cout << "2. Символы (char)" << std::endl;
    std::cout << "3. Числа с плавающей точкой (float)" << std::endl;
    std::cout << "4. Числа двойной точности (double)" << std::endl;
    std::cout << "5. Строки (char*)" << std::endl;
    std::cout << "0. Вернуться в главное меню" << std::endl;
    std::cout << "Выберите тип данных: ";
}

void show_tree_operations_menu() {
    std::cout << "\n--- Операции с деревом ---" << std::endl;
    std::cout << "1. Добавить элемент" << std::endl;
    std::cout << "2. Найти элемент" << std::endl;
    std::cout << "3. Показать структуру дерева" << std::endl;
    std::cout << "4. Обход в нисходящем порядке" << std::endl;
    std::cout << "5. Обход в восходящем порядке" << std::endl;
    std::cout << "6. Симметричный обход" << std::endl;
    std::cout << "7. Очистить дерево" << std::endl;
    std::cout << "0. Назад к выбору типа данных" << std::endl;
    std::cout << "Выберите действие: ";
}

template<typename T>
void handle_tree_operations(BinaryTree<T>& tree, const std::string& type_name) {
    int choice;
    
    do {
        show_tree_operations_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                T value;
                std::cout << "Введите значение (" << type_name << ") для добавления: ";
                std::cin >> value;
                tree.insert(value);
                std::cout << "Значение " << value << " добавлено в дерево." << std::endl;
                break;
            }
            
            case 2: {
                T value;
                std::cout << "Введите значение (" << type_name << ") для поиска: ";
                std::cin >> value;
                if (tree.search(value)) {
                    std::cout << "Значение " << value << " найдено в дереве." << std::endl;
                } else {
                    std::cout << "Значение " << value << " не найдено в дереве." << std::endl;
                }
                break;
            }
            
            case 3: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.print_tree();
                }
                break;
            }
            
            case 4: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.preorder_traversal();
                }
                break;
            }
            
            case 5: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.postorder_traversal();
                }
                break;
            }
            
            case 6: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.inorder_traversal();
                }
                break;
            }
            
            case 7: {
                tree.clear();
                std::cout << "Дерево очищено." << std::endl;
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

// Специализация для char*
template<>
void handle_tree_operations<char*>(BinaryTree<char*>& tree, const std::string& type_name) {
    int choice;
    
    do {
        show_tree_operations_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                char* value = new char[100];
                std::cout << "Введите строку (" << type_name << ") для добавления: ";
                std::cin >> value;
                
                // Создаем копию для дерева
                char* tree_value = new char[strlen(value) + 1];
                strcpy(tree_value, value);
                
                tree.insert(tree_value);
                std::cout << "Значение \"" << value << "\" добавлено в дерево." << std::endl;
                delete[] value;
                break;
            }
            
            case 2: {
                char* value = new char[100];
                std::cout << "Введите строку (" << type_name << ") для поиска: ";
                std::cin >> value;
                if (tree.search(value)) {
                    std::cout << "Значение \"" << value << "\" найдено в дереве." << std::endl;
                } else {
                    std::cout << "Значение \"" << value << "\" не найдено в дереве." << std::endl;
                }
                delete[] value;
                break;
            }
            
            case 3: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.print_tree();
                }
                break;
            }
            
            case 4: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.preorder_traversal();
                }
                break;
            }
            
            case 5: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.postorder_traversal();
                }
                break;
            }
            
            case 6: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.inorder_traversal();
                }
                break;
            }
            
            case 7: {
                tree.clear();
                std::cout << "Дерево очищено." << std::endl;
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

// Специализация для char
template<>
void handle_tree_operations<char>(BinaryTree<char>& tree, const std::string& type_name) {
    int choice;
    
    do {
        show_tree_operations_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                char value;
                std::cout << "Введите символ (" << type_name << ") для добавления: ";
                std::cin >> value;
                tree.insert(value);
                std::cout << "Значение '" << value << "' добавлено в дерево." << std::endl;
                break;
            }
            
            case 2: {
                char value;
                std::cout << "Введите символ (" << type_name << ") для поиска: ";
                std::cin >> value;
                if (tree.search(value)) {
                    std::cout << "Значение '" << value << "' найдено в дереве." << std::endl;
                } else {
                    std::cout << "Значение '" << value << "' не найдено в дереве." << std::endl;
                }
                break;
            }
            
            case 3: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.print_tree();
                }
                break;
            }
            
            case 4: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.preorder_traversal();
                }
                break;
            }
            
            case 5: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.postorder_traversal();
                }
                break;
            }
            
            case 6: {
                if (tree.is_empty()) {
                    std::cout << "Дерево пустое." << std::endl;
                } else {
                    tree.inorder_traversal();
                }
                break;
            }
            
            case 7: {
                tree.clear();
                std::cout << "Дерево очищено." << std::endl;
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

template<typename T>
void handle_linear_search_operations(std::vector<T>& current_array, T max_val, const std::string& type_name) {
    int choice;
    
    do {
        show_linear_search_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                int size = generate_random_size(30, 100);
                current_array = generate_random_array<T>(size, max_val);
                std::cout << "Сгенерирован новый массив типа " << type_name << " размером " << size << std::endl;
                print_array(current_array);
                break;
            }
            
            case 2: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    std::cout << "Текущий массив типа " << type_name << " размером " << current_array.size() << ":" << std::endl;
                    print_array(current_array);
                }
                break;
            }
            
            case 3: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    T value;
                    std::cout << "Введите значение (" << type_name << ") для поиска: ";
                    std::cin >> value;
                    demonstrate_search(current_array, value);
                }
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}


template<>
void handle_linear_search_operations<char*>(std::vector<char*>& current_array, char* max_val, const std::string& type_name) {
    int choice;
    
    do {
        show_linear_search_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {

                cleanup_char_array(current_array);
                
                int size = generate_random_size(30, 100);
                current_array = generate_random_array<char*>(size, max_val);
                std::cout << "Сгенерирован новый массив типа " << type_name << " размером " << size << std::endl;
                print_array(current_array);
                break;
            }
            
            case 2: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    std::cout << "Текущий массив типа " << type_name << " размером " << current_array.size() << ":" << std::endl;
                    print_array(current_array);
                }
                break;
            }
            
            case 3: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    char* value = new char[100];
                    std::cout << "Введите строку для поиска: ";
                    std::cin >> value;
                    demonstrate_search(current_array, value);
                    delete[] value;
                }
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}


template<>
void handle_linear_search_operations<char>(std::vector<char>& current_array, char max_val, const std::string& type_name) {
    int choice;
    
    do {
        show_linear_search_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                int size = generate_random_size(30, 100);
                current_array = generate_random_array<char>(size, max_val);
                std::cout << "Сгенерирован новый массив типа " << type_name << " размером " << size << std::endl;
                print_array(current_array);
                break;
            }
            
            case 2: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    std::cout << "Текущий массив типа " << type_name << " размером " << current_array.size() << ":" << std::endl;
                    print_array(current_array);
                }
                break;
            }
            
            case 3: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    char value;
                    std::cout << "Введите символ для поиска: ";
                    std::cin >> value;
                    demonstrate_search(current_array, value);
                }
                break;
            }
            
            case 0:
                std::cout << "Возврат к выбору типа данных..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

void handle_linear_search() {
    int choice;
    
    do {
        show_data_type_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                std::cout << "Работа с линейным поиском для типа int" << std::endl;
                handle_linear_search_operations(current_int_array, 100, "int");
                break;
            }
            
            case 2: {
                std::cout << "Работа с линейным поиском для типа char" << std::endl;
                handle_linear_search_operations(current_char_array, 'Z', "char");
                break;
            }
            
            case 3: {
                std::cout << "Работа с линейным поиском для типа float" << std::endl;
                handle_linear_search_operations(current_float_array, 100.0f, "float");
                break;
            }
            
            case 4: {
                std::cout << "Работа с линейным поиском для типа double" << std::endl;
                handle_linear_search_operations(current_double_array, 100.0, "double");
                break;
            }
            
            case 5: {
                std::cout << "Работа с линейным поиском для типа char*" << std::endl;
                char* dummy = nullptr;
                handle_linear_search_operations(current_charptr_array, dummy, "char*");
                break;
            }
            
            case 0:
                std::cout << "Возврат в главное меню..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

void handle_binary_tree() {
    int choice;
    
    do {
        show_tree_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                BinaryTree<int> int_tree;
                std::cout << "Работа с деревом целых чисел" << std::endl;
                handle_tree_operations(int_tree, "int");
                break;
            }
            
            case 2: {
                BinaryTree<char> char_tree;
                std::cout << "Работа с деревом символов" << std::endl;
                handle_tree_operations(char_tree, "char");
                break;
            }
            
            case 3: {
                BinaryTree<float> float_tree;
                std::cout << "Работа с деревом чисел float" << std::endl;
                handle_tree_operations(float_tree, "float");
                break;
            }
            
            case 4: {
                BinaryTree<double> double_tree;
                std::cout << "Работа с деревом чисел double" << std::endl;
                handle_tree_operations(double_tree, "double");
                break;
            }
            
            case 5: {
                BinaryTree<char*> charptr_tree;
                std::cout << "Работа с деревом строк" << std::endl;
                handle_tree_operations(charptr_tree, "char*");
                break;
            }
            
            case 0:
                std::cout << "Возврат в главное меню..." << std::endl;
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                break;
        }
        
    } while (choice != 0);
}

int main() {
    
    int main_choice;
    
    do {
        show_main_menu();
        std::cin >> main_choice;
        
        switch (main_choice) {
            case 1:
                handle_linear_search();
                break;
                
            case 2:
                handle_binary_tree();
                break;
                
            default:
                std::cout << "Неверный выбор." << std::endl;
                break;
        }
        
    } while (main_choice != 0);
    
    return 0;
}
