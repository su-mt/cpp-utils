#ifndef QUEUE1_PRIVATE_H
#define QUEUE1_PRIVATE_H

#include "queue.h"

class QueuePrivate : private Queue {
public:
    QueuePrivate();
    QueuePrivate(const QueuePrivate& other);
    QueuePrivate& operator=(const QueuePrivate& other);
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

#endif
