#include "include/long_number.hpp"
#include <iostream>


LongNumber::LongNumber() : value(0) {}

LongNumber::LongNumber(long val) : value(val) {}

LongNumber::LongNumber(const LongNumber& other) : value(other.value) {}


LongNumber& LongNumber::operator=(const LongNumber& other) {
    if (this != &other) {
        value = other.value;
    }
    return *this;
}

LongNumber& LongNumber::operator=(long val) {
    value = val;
    return *this;
}


LongNumber::~LongNumber() {}


long LongNumber::getValue() const {
    return value;
}


bool LongNumber::operator==(const LongNumber& other) const {
    return value == other.value;
}

bool LongNumber::operator==(long val) const {
    return value == val;
}

bool LongNumber::operator!=(const LongNumber& other) const {
    return value != other.value;
}

bool LongNumber::operator!=(long val) const {
    return value != val;
}

bool LongNumber::operator<(const LongNumber& other) const {
    return value < other.value;
}

bool LongNumber::operator<(long val) const {
    return value < val;
}




bool operator>(const LongNumber& lhs, const LongNumber& rhs) {
    return lhs.value > rhs.value;
}

bool operator>(const LongNumber& lhs, long rhs) {
    return lhs.value > rhs;
}

bool operator>(long lhs, const LongNumber& rhs) {
    return lhs > rhs.value;
}


bool operator<=(const LongNumber& lhs, const LongNumber& rhs) {
    return lhs.value <= rhs.value;
}

bool operator<=(const LongNumber& lhs, long rhs) {
    return lhs.value <= rhs;
}

bool operator<=(long lhs, const LongNumber& rhs) {
    return lhs <= rhs.value;
}


bool operator>=(const LongNumber& lhs, const LongNumber& rhs) {
    return lhs.value >= rhs.value;
}

bool operator>=(const LongNumber& lhs, long rhs) {
    return lhs.value >= rhs;
}

bool operator>=(long lhs, const LongNumber& rhs) {
    return lhs >= rhs.value;
}


bool operator==(long lhs, const LongNumber& rhs) {
    return lhs == rhs.value;
}

bool operator!=(long lhs, const LongNumber& rhs) {
    return lhs != rhs.value;
}

bool operator<(long lhs, const LongNumber& rhs) {
    return lhs < rhs.value;
}


void LongNumber::print() const {
    std::cout << value;
}
