// Очередь с приватным наследованием (полностью скрывает базовый класс)
#ifndef QUEUE1_PRIVATE_H
#define QUEUE1_PRIVATE_H

#include "queue.h"

class QueuePrivate : private Queue {
public:
    // Конструктор
    QueuePrivate();
    
    // Конструктор копирования
    QueuePrivate(const QueuePrivate& other);
    
    // Оператор присваивания
    QueuePrivate& operator=(const QueuePrivate& other);
    
    // Деструктор
    ~QueuePrivate();
    
    // Свои методы (заново объявляем интерфейс)
    void add(int value);
    int take();
    bool empty() const;
    void show() const;
    void clear();
    int size() const;
    
    // Создать копию
    QueuePrivate copy() const;
    
    // Объединить две очереди
    static QueuePrivate merge(const QueuePrivate& q1, const QueuePrivate& q2);
    
    // Вычислить сумму чисел, которые меньше среднего гармонического
    double calculateSum();

private:
    // Вычислить среднее гармоническое всех элементов
    double getHarmonicMean();
};

#endif
