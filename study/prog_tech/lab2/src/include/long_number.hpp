#pragma once

class LongNumber {
private:
    long value;

public:
    // Конструкторы
    LongNumber();
    LongNumber(long val);
    LongNumber(const LongNumber& other);
    
    // Оператор присваивания
    LongNumber& operator=(const LongNumber& other);
    LongNumber& operator=(long val);
    
    // Деструктор
    ~LongNumber();
    
    // Геттер
    long getValue() const;
    
    // Операторы сравнения (методы класса)
    bool operator==(const LongNumber& other) const;
    bool operator==(long val) const;
    bool operator!=(const LongNumber& other) const;
    bool operator!=(long val) const;
    bool operator<(const LongNumber& other) const;
    bool operator<(long val) const;
    
    // Дружественные функции для операторов сравнения
    friend bool operator>(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>(const LongNumber& lhs, long rhs);
    friend bool operator>(long lhs, const LongNumber& rhs);
    
    friend bool operator<=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator<=(const LongNumber& lhs, long rhs);
    friend bool operator<=(long lhs, const LongNumber& rhs);
    
    friend bool operator>=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>=(const LongNumber& lhs, long rhs);
    friend bool operator>=(long lhs, const LongNumber& rhs);
    
    // Дружественные функции для симметричных операторов == и !=
    friend bool operator==(long lhs, const LongNumber& rhs);
    friend bool operator!=(long lhs, const LongNumber& rhs);
    friend bool operator<(long lhs, const LongNumber& rhs);
    
    // Функция для вывода
    void print() const;
};
