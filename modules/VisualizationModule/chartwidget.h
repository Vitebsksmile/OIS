#pragma once
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

//using namespace QtCharts;

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    void updateMetrics(double precision, double recall, double f1);

private:
    QChart *chart;
    QChartView *chartView;
    QBarSet *metricsSet;
};
