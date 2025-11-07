#include "visualizer.h"

Visualizer::Visualizer(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    imageViewer = new ImageViewerWidget(this);
    chartWidget = new ChartWidget(this);
    statsWidget = new StatisticsWidget(this);
    exportManager = new ExportManager(this);

    layout->addWidget(imageViewer, 5);
    layout->addWidget(chartWidget, 2);
    layout->addWidget(statsWidget, 1);
    setLayout(layout);
}

void Visualizer::displayImage(const QImage &image)
{
    imageViewer->displayImage(image);
}

void Visualizer::displayDetections(const QVector<QRect> &boxes)
{
    imageViewer->drawDetections(boxes);
}

void Visualizer::updateStatistics(double precision, double recall, double f1)
{
    chartWidget->updateMetrics(precision, recall, f1);
    statsWidget->setMetrics(precision, recall, f1);
}
