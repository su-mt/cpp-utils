#include "include/long_number.hpp"
#include <iostream>

void testComparisons() {
    std::cout << "=== Тестирование операторов сравнения ===" << std::endl;
    
    LongNumber a(10);
    LongNumber b(20);
    LongNumber c(10);
    long x = 15;
    
    std::cout << "a = "; a.print(); std::cout << ", b = "; b.print(); 
    std::cout << ", c = "; c.print(); std::cout << ", x = " << x << std::endl;
    
    // Тестирование операторов == (методы)
    std::cout << "\n--- Операторы == (методы) ---" << std::endl;
    std::cout << "a == c: " << (a == c) << std::endl;
    std::cout << "a == b: " << (a == b) << std::endl;
    std::cout << "a == 10: " << (a == 10) << std::endl;
    std::cout << "10 == a: " << (10 == a) << std::endl;
    
    // Тестирование операторов != (методы)
    std::cout << "\n--- Операторы != (методы) ---" << std::endl;
    std::cout << "a != b: " << (a != b) << std::endl;
    std::cout << "a != c: " << (a != c) << std::endl;
    std::cout << "a != 15: " << (a != 15) << std::endl;
    std::cout << "15 != a: " << (15 != a) << std::endl;
    
    // Тестирование операторов < (методы)
    std::cout << "\n--- Операторы < (методы) ---" << std::endl;
    std::cout << "a < b: " << (a < b) << std::endl;
    std::cout << "b < a: " << (b < a) << std::endl;
    std::cout << "a < 15: " << (a < 15) << std::endl;
    std::cout << "15 < a: " << (15 < a) << std::endl;
    
    // Тестирование операторов > (дружественные функции)
    std::cout << "\n--- Операторы > (дружественные функции) ---" << std::endl;
    std::cout << "b > a: " << (b > a) << std::endl;
    std::cout << "a > b: " << (a > b) << std::endl;
    std::cout << "a > 5: " << (a > 5) << std::endl;
    std::cout << "5 > a: " << (5 > a) << std::endl;
    
    // Тестирование операторов <= (дружественные функции)
    std::cout << "\n--- Операторы <= (дружественные функции) ---" << std::endl;
    std::cout << "a <= c: " << (a <= c) << std::endl;
    std::cout << "a <= b: " << (a <= b) << std::endl;
    std::cout << "a <= 10: " << (a <= 10) << std::endl;
    std::cout << "10 <= a: " << (10 <= a) << std::endl;
    
    // Тестирование операторов >= (дружественные функции)
    std::cout << "\n--- Операторы >= (дружественные функции) ---" << std::endl;
    std::cout << "b >= a: " << (b >= a) << std::endl;
    std::cout << "a >= c: " << (a >= c) << std::endl;
    std::cout << "a >= 10: " << (a >= 10) << std::endl;
    std::cout << "10 >= a: " << (10 >= a) << std::endl;
}

int main() {
    testComparisons();
    return 0;
}
