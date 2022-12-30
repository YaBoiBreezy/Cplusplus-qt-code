#include "UI/mainwindow.h"
#include "Controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller c;
    MainWindow w(&c);
    c.setWindow(&w); //controller and mainWindow need pointers to each other
    w.show();
    return a.exec();
}
