#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class StatisticsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatisticsWidget(QWidget *parent = nullptr);
    void setMetrics(double precision, double recall, double f1);

private:
    QLabel *precisionLabel;
    QLabel *recallLabel;
    QLabel *f1Label;
};
