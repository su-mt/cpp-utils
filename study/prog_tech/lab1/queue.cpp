#include "include/queue.h"
#include <iostream>
#include <stdexcept>

Queue::Queue() {
    head = nullptr;
    tail = nullptr;
    count = 0;
}

Queue::Queue(const Queue& other) {
    head = nullptr;
    tail = nullptr;
    count = 0;
    Node* current = other.head;
    while (current != nullptr) {
        add(current->data);
        current = current->next;
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this == &other) return *this; // Самоприсваивание
    
    clear(); // Очищаем текущие данные
    
    Node* current = other.head;
    while (current != nullptr) {
        add(current->data);
        current = current->next;
    }
    return *this;
}

Queue::~Queue() {
    clear();
}

void Queue::add(int value) {
    Node* newNode = new Node(value);
    if (empty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

int Queue::take() {
    if (empty()) throw std::runtime_error("Очередь пуста!");
    int value = head->data;
    Node* temp = head;
    head = head->next;
    if (head == nullptr) tail = nullptr;
    delete temp;
    count--;
    return value;
}

bool Queue::empty() const {
    return head == nullptr;
}

int Queue::size() const {
    return count;
}

void Queue::show() const {
    if (empty()) {
        std::cout << "Очередь пуста" << std::endl;
        return;
    }
    std::cout << "Очередь: ";
    Node* current = head;
    while (current != nullptr) {
        std::cout << current->data;
        if (current->next != nullptr) std::cout << " <- ";
        current = current->next;
    }
    std::cout << std::endl;
}

void Queue::clear() {
    while (!empty()) take();
}

Queue Queue::copy() const {
    return Queue(*this);
}

Queue Queue::merge(const Queue& q1, const Queue& q2) {
    Queue result;
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