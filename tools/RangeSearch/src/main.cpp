#include "gui.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    rsgui::RangeSearchGUI w;
    w.show();
    return app.exec();
}