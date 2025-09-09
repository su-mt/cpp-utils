#pragma once
#include "queue.h"

class QueueProtected : protected Queue {
public:
    QueueProtected();
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
