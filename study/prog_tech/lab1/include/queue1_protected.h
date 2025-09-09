// Очередь с защищенным наследованием (нужно открывать доступ к методам)
#ifndef QUEUE1_PROTECTED_H
#define QUEUE1_PROTECTED_H

#include "queue.h"

class QueueProtected : protected Queue {
public:
    // Конструктор
    QueueProtected();
    
    // Конструктор копирования
    QueueProtected(const QueueProtected& other);
    
    // Оператор присваивания
    QueueProtected& operator=(const QueueProtected& other);
    
    // Деструктор
    ~QueueProtected();
    
    // Открываем доступ к методам базового класса
    void add(int value);
    int take();
    bool empty() const;
    void show() const;
    void clear();
    int size() const;
    
    // Создать копию
    QueueProtected copy() const;
    
    // Объединить две очереди
    static QueueProtected merge(const QueueProtected& q1, const QueueProtected& q2);
    
    // Вычислить сумму чисел, которые меньше среднего гармонического
    double calculateSum();

private:
    // Вычислить среднее гармоническое всех элементов
    double getHarmonicMean();
};

#endif
