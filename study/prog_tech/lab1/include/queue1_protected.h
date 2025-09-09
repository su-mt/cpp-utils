#ifndef QUEUE1_PROTECTED_H
#define QUEUE1_PROTECTED_H

#include "queue.h"

class QueueProtected : protected Queue {
public:
    QueueProtected();
    QueueProtected(const QueueProtected& other);
    QueueProtected& operator=(const QueueProtected& other);
    ~QueueProtected();
    void add(int value);
    int take();
    bool empty() const;
    void show() const;
    void clear();
    int size() const;
    QueueProtected copy() const;
    static QueueProtected merge(const QueueProtected& q1, const QueueProtected& q2);
    double calculateSum();

private:
    double getHarmonicMean();
};

#endif
