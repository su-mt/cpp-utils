#ifndef MENU_H
#define MENU_H

#include "figure.h"
#include "circle.h"
#include "rectangle.h"
#include "trapezoid.h"
#include "triangle.h"
#include <vector>
#include <memory>
#include <string>



class Menu {
private:
    std::vector<std::unique_ptr<Figure>> figures;
    
    void showMainMenu() const;

    

    void addFigure();

    void createCircle();
    
    void createRectangle();
    
    void createTrapezoid();

    void createTriangle();

    void showAllFigures() const;
    
    void showFigure();
    
    void deleteFigure();
    
    void saveToFile() const;
    
    void loadFromFile();
    
    void saveResultsToFile() const;
    
    double getValidDouble(const std::string& prompt) const;
    
    int getValidInt(const std::string& prompt) const;
    
    std::string getFileName(const std::string& prompt) const;
    
public:

    void run();
};

#endif // MENU_H
