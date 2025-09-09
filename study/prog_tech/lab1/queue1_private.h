#pragma once
#include "include/queue.h"

class QueuePrivate : private Queue {
public:
    QueuePrivate();
    ~QueuePrivate();
    void add(int value);
    int take();
    bool empty() const;
    void show() const;
    void clear();
    int size() const;
    QueuePrivate copy() const;
    static QueuePrivate merge(const QueuePrivate& q1, const QueuePrivate& q2);
    double calculateSum();
private:
    double getHarmonicMean();
};
