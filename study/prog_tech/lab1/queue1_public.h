#pragma once
#include "include/queue.h"

class QueuePublic : public Queue {
public:
    QueuePublic();
    ~QueuePublic();
    QueuePublic copy() const;
    static QueuePublic merge(const QueuePublic& q1, const QueuePublic& q2);
    double calculateSum();
private:
    double getHarmonicMean();
};
