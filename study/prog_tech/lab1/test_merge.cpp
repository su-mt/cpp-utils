#include "include/queue1_public.h"
#include <iostream>

int main() {
    std::cout << "=== Тестирование слияния очередей ===" << std::endl;
    
    try {
        QueuePublic queue1, queue2;
        
        std::cout << "Добавляем в первую очередь: 1, 2, 3" << std::endl;
        queue1.add(1);
        queue1.add(2);
        queue1.add(3);
        
        std::cout << "Добавляем во вторую очередь: 4, 5, 6" << std::endl;
        queue2.add(4);
        queue2.add(5);
        queue2.add(6);
        
        std::cout << "\nПервая очередь:" << std::endl;
        queue1.show();
        
        std::cout << "Вторая очередь:" << std::endl;
        queue2.show();
        
        std::cout << "\nСливаем очереди..." << std::endl;
        QueuePublic mergedQueue = QueuePublic::merge(queue1, queue2);
        
        std::cout << "Результат слияния:" << std::endl;
        mergedQueue.show();
        
        std::cout << "\nПроверяем, что оригинальные очереди не повреждены:" << std::endl;
        std::cout << "Первая очередь:" << std::endl;
        queue1.show();
        
        std::cout << "Вторая очередь:" << std::endl;
        queue2.show();
        
        std::cout << "\n=== Тест успешно завершен ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
