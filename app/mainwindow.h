#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //    Конструктор и деструктор (только объявления)
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPushButton *btn1, *btn2;   // указатель на кнопку

    // Панели инструментов
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    // Приватный указатель на пользовательский виджет:
//    ImageViewer *imageViewer;

    // Объявление приватного метода для инициализации
    // и компоновки всего интерфейса.
    void setupUI();
};

#endif // MAINWINDOW_H
