#include "include/linear_search.h"
#include "include/binary_tree.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<int> current_array;

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

void show_linear_search_menu() {
    std::cout << "\n--- Меню линейного поиска ---" << std::endl;
    std::cout << "1. Сгенерировать новый массив" << std::endl;
    std::cout << "2. Показать текущий массив" << std::endl;
    std::cout << "3. Найти элемент в массиве" << std::endl;
    std::cout << "0. Вернуться в главное меню" << std::endl;
    std::cout << "Выберите действие: ";
}

void show_tree_menu() {
    std::cout << "\n--- Меню бинарного дерева ---" << std::endl;
    std::cout << "1. Работа с целыми числами" << std::endl;
    std::cout << "2. Работа со строками" << std::endl;
    std::cout << "3. Работа с числами float" << std::endl;
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
void handle_linear_search() {
    int choice;
    
    do {
        show_linear_search_menu();
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                int size = generate_random_size(30, 100);
                current_array = generate_random_array<int>(size, 100);
                std::cout << "Сгенерирован новый массив размером " << size << std::endl;
                print_array(current_array);
                break;
            }
            
            case 2: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    std::cout << "Текущий массив размером " << current_array.size() << ":" << std::endl;
                    print_array(current_array);
                }
                break;
            }
            
            case 3: {
                if (current_array.empty()) {
                    std::cout << "Массив пустой. Сначала сгенерируйте массив." << std::endl;
                } else {
                    int value;
                    std::cout << "Введите значение для поиска: ";
                    std::cin >> value;
                    demonstrate_search(current_array, value);
                }
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
                BinaryTree<std::string> string_tree;
                std::cout << "Работа с деревом строк" << std::endl;
                handle_tree_operations(string_tree, "string");
                break;
            }
            
            case 3: {
                BinaryTree<float> float_tree;
                std::cout << "Работа с деревом чисел float" << std::endl;
                handle_tree_operations(float_tree, "float");
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
    std::cout << "Добро пожаловать в программу демонстрации алгоритмов!" << std::endl;
    
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
