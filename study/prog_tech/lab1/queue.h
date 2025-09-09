#pragma once
#include <iostream>

struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

class Queue {
protected:
    Node* head;
    Node* tail;
    int count;
public:
    Queue();
    Queue(const Queue& other);
    virtual ~Queue();
    void add(int value);
    int take();
    bool empty() const;
    int size() const;
    void show() const;
    void clear();
    Queue copy() const;
    static Queue merge(const Queue& q1, const Queue& q2);
};