// Очередь с публичным наследованием (все методы доступны)
#ifndef QUEUE1_PUBLIC_H
#define QUEUE1_PUBLIC_H

#include "queue.h"

class QueuePublic : public Queue {
public:
    // Конструктор
    QueuePublic();
    
    // Конструктор копирования
    QueuePublic(const QueuePublic& other);
    
    // Оператор присваивания
    QueuePublic& operator=(const QueuePublic& other);
    
    // Деструктор
    ~QueuePublic();
    
    // Создать копию
    QueuePublic copy() const;
    
    // Объединить две очереди
    static QueuePublic merge(const QueuePublic& q1, const QueuePublic& q2);
    
    // Вычислить сумму чисел, которые меньше среднего гармонического
    double calculateSum();

private:
    // Вычислить среднее гармоническое всех элементов
    double getHarmonicMean();
};

#endif
