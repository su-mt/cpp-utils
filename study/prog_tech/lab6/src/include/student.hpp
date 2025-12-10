#include <string>
#include <vector>
#include <iostream>
#include "exceptions.hpp"

namespace STUDENT {

class Student {
private:
    std::string surname;              
    int group_number;                 
    std::vector<int> marks;           
    
    
    void validateMark(int mark) const;
    
public:
    Student(const std::string& surname_initials = "", int group_number = 0);
    
    
    std::string getSurnameInitials() const;
    int getGroupNumber() const;
    std::vector<int> getMarks() const;
    double getAverageMark() const;
    
    
    void setSurnameInitials(const std::string& surname_initials);
    void setGroupNumber(int group_number);
    void setMarks(const std::vector<int>& marks);
    void addMark(int mark);
    
    
    bool operator<(const Student& other) const;
    bool operator==(const Student& other) const;
    
    
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);
};


struct StudentComparator {
    bool operator()(const Student& a, const Student& b) const;
};

} // namespace STUDENT