#include <QApplication>
#include <QWidget>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    QWidget widget;
    widget.show();

    std::cout << "YES" << std::endl;

    return a.exec();
}
