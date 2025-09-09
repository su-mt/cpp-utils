#include "include/queue1_private.h"
#include <iostream>
#include <stdexcept>

QueuePrivate::QueuePrivate() : Queue() {}
QueuePrivate::QueuePrivate(const QueuePrivate& other) : Queue(other) {}
QueuePrivate& QueuePrivate::operator=(const QueuePrivate& other) {
    if (this == &other) return *this;
    Queue::operator=(other);
    return *this;
}
QueuePrivate::~QueuePrivate() {}
void QueuePrivate::add(int value) { Queue::add(value); }
int QueuePrivate::take() { return Queue::take(); }
bool QueuePrivate::empty() const { return Queue::empty(); }
void QueuePrivate::show() const { Queue::show(); }
void QueuePrivate::clear() { Queue::clear(); }
int QueuePrivate::size() const { return Queue::size(); }

QueuePrivate QueuePrivate::copy() const {
    QueuePrivate result;
    Node* current = head;
    while (current != nullptr) {
        result.add(current->data);
        current = current->next;
    }
    return result;
}

QueuePrivate QueuePrivate::merge(const QueuePrivate& q1, const QueuePrivate& q2) {
    QueuePrivate result;
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

double QueuePrivate::getHarmonicMean() {
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

double QueuePrivate::calculateSum() {
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
