
#pragma once

class Queue {
private:
	struct Node {
		int data;
		Node* next;
		explicit Node(int v) : data(v), next(nullptr) {}
	};

	Node* head;
	Node* tail;
	int count;
	int userStep;  // пользовательское значение для операторов ++ и --

public:
	Queue();
	Queue(const Queue& other);
	Queue& operator=(const Queue& other);
	~Queue();

	void add(int value);
	int take();
	bool empty() const;
	int size() const;
	void show() const;
	void clear();
	void init(int n);
	void initRandom(int n, int minVal = 0, int maxVal = 100);
	void setUserStep(int step);  // установка пользовательского значения
	int getUserStep() const;     // получение пользовательского значения

	// Перегруженные операторы
	Queue& operator++();           // префиксная форма ++q
	Queue& operator--();           // префиксная форма --q
	
	// Дружественные функции для постфиксных операторов
	friend Queue operator++(Queue& q, int);  // постфиксная форма q++
	friend Queue operator--(Queue& q, int);  // постфиксная форма q--

private:
	// Вспомогательные методы для операторов
	int getMin() const;
	int getMax() const;

};

