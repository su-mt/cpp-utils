#include "include/queue1_protected.h"
#include <iostream>
#include <stdexcept>

QueueProtected::QueueProtected() : Queue() {}
QueueProtected::QueueProtected(const QueueProtected& other) : Queue(other) {}
QueueProtected& QueueProtected::operator=(const QueueProtected& other) {
    if (this == &other) return *this;
    Queue::operator=(other);
    return *this;
}
QueueProtected::~QueueProtected() {}
void QueueProtected::add(int value) { Queue::add(value); }
int QueueProtected::take() { return Queue::take(); }
bool QueueProtected::empty() const { return Queue::empty(); }
void QueueProtected::show() const { Queue::show(); }
void QueueProtected::clear() { Queue::clear(); }
int QueueProtected::size() const { return Queue::size(); }

QueueProtected QueueProtected::copy() const {
    QueueProtected result;
    Node* current = head;
    while (current != nullptr) {
        result.add(current->data);
        current = current->next;
    }
    return result;
}

QueueProtected QueueProtected::merge(const QueueProtected& q1, const QueueProtected& q2) {
    QueueProtected result;
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

double QueueProtected::getHarmonicMean() {
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

double QueueProtected::calculateSum() {
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
