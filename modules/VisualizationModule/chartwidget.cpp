#include "chartwidget.h"

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    metricsSet = new QBarSet("Metrics");
    *metricsSet << 0 << 0 << 0;

    auto *series = new QBarSeries();
    series->append(metricsSet);

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Model Performance Metrics");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "Precision" << "Recall" << "F1-Score";
    auto *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto *axisY = new QValueAxis();
    axisY->setRange(0, 1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

void ChartWidget::updateMetrics(double precision, double recall, double f1)
{
    *metricsSet << 0 << 0 << 0; // clear
    metricsSet->replace(0, precision);
    metricsSet->replace(1, recall);
    metricsSet->replace(2, f1);
}
