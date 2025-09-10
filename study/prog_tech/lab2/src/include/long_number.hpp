#pragma once

class LongNumber {
private:
    long value;

public:

    LongNumber();
    LongNumber(long val);
    LongNumber(const LongNumber& other);
    

    LongNumber& operator=(const LongNumber& other);
    LongNumber& operator=(long val);
    

    ~LongNumber();
    

    long getValue() const;
    

    bool operator==(const LongNumber& other) const;
    bool operator==(long val) const;
    bool operator!=(const LongNumber& other) const;
    bool operator!=(long val) const;
    bool operator<(const LongNumber& other) const;
    bool operator<(long val) const;
    

    friend bool operator>(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>(const LongNumber& lhs, long rhs);
    friend bool operator>(long lhs, const LongNumber& rhs);
    
    friend bool operator<=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator<=(const LongNumber& lhs, long rhs);
    friend bool operator<=(long lhs, const LongNumber& rhs);
    
    friend bool operator>=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>=(const LongNumber& lhs, long rhs);
    friend bool operator>=(long lhs, const LongNumber& rhs);
    

    friend bool operator==(long lhs, const LongNumber& rhs);
    friend bool operator!=(long lhs, const LongNumber& rhs);
    friend bool operator<(long lhs, const LongNumber& rhs);
    

    void print() const;
};
