// Простая очередь для студентов
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

// Элемент очереди - простая структура
struct Node {
    int data;
    Node* next;
    
    Node(int value) : data(value), next(nullptr) {}
};

// Базовая очередь (FIFO - первый вошел, первый вышел)
class Queue {
protected:
    Node* head;  // Начало очереди (откуда берем)
    Node* tail;  // Конец очереди (куда добавляем)
    int count;   // Количество элементов

public:
    // Конструктор - создаем пустую очередь
    Queue();
    
    // Конструктор копирования
    Queue(const Queue& other);
    
    // Оператор присваивания
    Queue& operator=(const Queue& other);
    
    // Деструктор - освобождаем память
    virtual ~Queue();
    
    // Добавить элемент в очередь
    void add(int value);
    
    // Взять элемент из очереди
    int take();
    
    // Проверить, пуста ли очередь
    bool empty() const;
    
    // Получить размер очереди
    int size() const;
    
    // Показать содержимое очереди
    void show() const;
    
    // Очистить очередь
    void clear();
    
    // Создать копию очереди
    Queue copy() const;
    
    // Объединить две очереди
    static Queue merge(const Queue& q1, const Queue& q2);
};

#endif