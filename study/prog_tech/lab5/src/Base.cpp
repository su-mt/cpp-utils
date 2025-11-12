#include "include/Base.h"

Base::Base() : cost(0.0) {}

Base::Base(double cost) : cost(cost) {}

double Base::getCost() const {
    return cost;
}

void Base::setCost(double cost) {
    this->cost = cost;
}

std::ostream& operator<<(std::ostream& os, const Base& obj) {
    obj.display();
    return os;
}

std::istream& operator>>(std::istream& is, Base& obj) {
    std::cout << "Введите стоимость: ";
    is >> obj.cost;
    return is;
}