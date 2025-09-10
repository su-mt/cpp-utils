#include "include/queue.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

Queue::Queue() : head(nullptr), tail(nullptr), count(0), userStep(1) {
    
	static bool initialized = false;
	if (!initialized) {
		srand(static_cast<unsigned int>(time(nullptr)));
		initialized = true;
	}
}

Queue::Queue(const Queue& other) : head(nullptr), tail(nullptr), count(0), userStep(other.userStep) {
	Node* current = other.head;
	while (current != nullptr) {
		add(current->data);
		current = current->next;
	}
}

Queue& Queue::operator=(const Queue& other) {
	if (this == &other) return *this;
	clear();
	userStep = other.userStep;
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


void Queue::init(int n) {
	if (n < 0) throw std::invalid_argument("Размер не может быть отрицательным");
	clear();
	for (int i = 0; i < n; ++i) add(0);
}


void Queue::initRandom(int n, int minVal, int maxVal) {
	if (n < 0) throw std::invalid_argument("Размер не может быть отрицательным");
	if (minVal > maxVal) throw std::invalid_argument("minVal не может быть больше maxVal");
	clear();
	for (int i = 0; i < n; ++i) {
		int randomValue = minVal + rand() % (maxVal - minVal + 1);
		add(randomValue);
	}
}

// Методы для работы с пользовательским значением
void Queue::setUserStep(int step) {
	if (step < 0) throw std::invalid_argument("Шаг не может быть отрицательным");
	userStep = step;
}

int Queue::getUserStep() const {
	return userStep;
}

// Префиксная форма ++ (метод) - увеличивает размер очереди на userStep элементов
Queue& Queue::operator++() {
	for (int i = 0; i < userStep; ++i) {
	int randomValue = 1 + rand() % 100;
		add(randomValue);
	}
	return *this;
}

// Постфиксная форма ++ (дружественная функция) - увеличивает все данные на минимальное число
Queue operator++(Queue& q, int) {
	Queue temp = q;
	if (!q.empty()) {
		int minVal = q.getMin();
		Queue::Node* current = q.head;
		while (current != nullptr) {
			current->data += minVal;
			current = current->next;
		}
	}
	return temp;
}

// Префиксная форма -- (метод) - уменьшает размер очереди на userStep элементов
Queue& Queue::operator--() {
	for (int i = 0; i < userStep && !empty(); ++i) {
		take();
	}
	return *this;
}

// Постфиксная форма -- (дружественная функция) - уменьшает все данные на максимальное число
Queue operator--(Queue& q, int) {
	Queue temp = q;
	if (!q.empty()) {
		int maxVal = q.getMax();
		Queue::Node* current = q.head;
		while (current != nullptr) {
			current->data -= maxVal;
			current = current->next;
		}
	}
	return temp;
}

// Вспомогательные методы
int Queue::getMin() const {
	if (empty()) throw std::runtime_error("Очередь пуста!");
	int minVal = head->data;
	Node* current = head->next;
	while (current != nullptr) {
		if (current->data < minVal) {
			minVal = current->data;
		}
		current = current->next;
	}
	return minVal;
}

int Queue::getMax() const {
	if (empty()) throw std::runtime_error("Очередь пуста!");
	int maxVal = head->data;
	Node* current = head->next;
	while (current != nullptr) {
		if (current->data > maxVal) {
			maxVal = current->data;
		}
		current = current->next;
	}
	return maxVal;
}


