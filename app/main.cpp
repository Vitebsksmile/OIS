#include <QApplication>
#include <iostream>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainWindow widget;
    widget.setWindowTitle("OIS");
    widget.show();

    std::cout << "YES" << std::endl;

    return a.exec();
}
