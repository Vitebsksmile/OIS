#include <iostream>
#include "mainwindow.h"

// 1. Реализация конструктора
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) // Инициализируем базовый класс
{
    setMinimumSize(300,300);
    setupUI();
}

// 3. Реализация деструктора
MainWindow::~MainWindow()
{
    // Qt обычно сам заботится об удалении дочерних виджетов, но деструктор нужен.
}

void MainWindow::setupUI()
{
    QSplitter *centralSplitter = new QSplitter(Qt::Horizontal);

    btn1 = new QPushButton("кнопка 1", this);
    btn2 = new QPushButton("кнопка 2", this);

    centralSplitter->addWidget(btn1);
    centralSplitter->addWidget(btn2);

    setCentralWidget(centralSplitter);
}


