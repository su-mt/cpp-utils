
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
	int userStep;

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
	void setUserStep(int step);
	int getUserStep() const;

	Queue& operator++();
	Queue& operator--();
	
	friend Queue operator++(Queue& q, int);
	friend Queue operator--(Queue& q, int);

private:
	int getMin() const;
	int getMax() const;

};

