#ifndef STATIONERY_H
#define STATIONERY_H

#include "Base.h"
#include <string>


class Stationery : public Base {
private:
    std::string type;      
    std::string color;     
    std::string purpose;   

public:

    Stationery();
    Stationery(const std::string& type, const std::string& color, 
               const std::string& purpose, double cost);
    

    virtual ~Stationery() = default;
    

    virtual void display() const override;
    virtual Base* clone() const override;
    virtual void saveToFile(std::ofstream& file) const override;
    virtual void loadFromFile(std::ifstream& file) override;
    virtual std::string getType() const override;
    

    std::string getStationeryType() const;
    std::string getColor() const;
    std::string getPurpose() const;
    

    void setStationeryType(const std::string& type);
    void setColor(const std::string& color);
    void setPurpose(const std::string& purpose);
    

    friend std::ostream& operator<<(std::ostream& os, const Stationery& stationery);
    friend std::istream& operator>>(std::istream& is, Stationery& stationery);
};

#endif // STATIONERY_H