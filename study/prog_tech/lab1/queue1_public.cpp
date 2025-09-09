#include "include/queue1_public.h"
#include <iostream>
#include <stdexcept>

QueuePublic::QueuePublic() : Queue() {}
QueuePublic::QueuePublic(const QueuePublic& other) : Queue(other) {}
QueuePublic& QueuePublic::operator=(const QueuePublic& other) {
    if (this == &other) return *this;
    Queue::operator=(other);
    return *this;
}
QueuePublic::~QueuePublic() {}

double QueuePublic::getHarmonicMean() {
    if (empty()) throw std::runtime_error("Очередь пуста!");
    double sum = 0.0;
    int elementsCount = 0;
    Node* current = head;
    while (current != nullptr) {
        if (current->data == 0) throw std::runtime_error("Нельзя вычислить среднее гармоническое - есть ноль!");
        sum += 1.0 / current->data;
        elementsCount++;
        current = current->next;
    }
    return elementsCount / sum;
}

double QueuePublic::calculateSum() {
    if (empty()) {
        std::cout << "Очередь пуста!" << std::endl;
        return 0.0;
    }
    try {
        double harmonicMean = getHarmonicMean();
        std::cout << "Среднее гармоническое: " << harmonicMean << std::endl;
        double sum = 0.0;
        Node* current = head;
        while (current != nullptr) {
            if (current->data < harmonicMean) sum += current->data;
            current = current->next;
        }
        return sum;
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        return 0.0;
    }
}

QueuePublic QueuePublic::copy() const {
    QueuePublic result;
    Node* current = head;
    while (current != nullptr) {
        result.add(current->data);
        current = current->next;
    }
    return result;
}

QueuePublic QueuePublic::merge(const QueuePublic& q1, const QueuePublic& q2) {
    QueuePublic result;
    Node* current = q1.head;
    while (current != nullptr) {
        result.add(current->data);
        current = current->next;
    }
    current = q2.head;
    while (current != nullptr) {
        result.add(current->data);
        current = current->next;
    }
    return result;
}
