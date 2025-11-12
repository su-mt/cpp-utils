#ifndef TEXTBOOK_H
#define TEXTBOOK_H

#include "Base.h"
#include <string>


class Textbook : public Base {
private:
    std::string title;          
    std::string author;         
    int year;                  
    std::string institution;   
    int studyYear;             
    int pages;                 

public:

    Textbook();
    Textbook(const std::string& title, const std::string& author, int year, 
             const std::string& institution, int studyYear, int pages, double cost);
    

    virtual ~Textbook() = default;
    

    virtual void display() const override;
    virtual Base* clone() const override;
    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;
    virtual std::string getType() const override;
    

    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getInstitution() const;
    int getStudyYear() const;
    int getPages() const;
    

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setInstitution(const std::string& institution);
    void setStudyYear(int studyYear);
    void setPages(int pages);
    

    friend std::ostream& operator<<(std::ostream& os, const Textbook& textbook);
    friend std::istream& operator>>(std::istream& is, Textbook& textbook);
};

#endif // TEXTBOOK_H