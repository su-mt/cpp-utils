#ifndef QUEUE1_PUBLIC_H
#define QUEUE1_PUBLIC_H

#include "queue.h"

class QueuePublic : public Queue {
public:
    QueuePublic();
    QueuePublic(const QueuePublic& other);
    QueuePublic& operator=(const QueuePublic& other);
    ~QueuePublic();
    QueuePublic copy() const;
    static QueuePublic merge(const QueuePublic& q1, const QueuePublic& q2);
    double calculateSum();

private:
    double getHarmonicMean();
};

#endif
