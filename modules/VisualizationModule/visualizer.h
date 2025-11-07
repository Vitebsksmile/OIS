#pragma once
#include "visualizationmodule_global.h"
#include "imageviewerwidget.h"
#include "chartwidget.h"
#include "statisticswidget.h"
#include "exportmanager.h"
#include <QWidget>
#include <QVBoxLayout>

class Visualizer : public QWidget
{
    Q_OBJECT
public:
    explicit Visualizer(QWidget *parent = nullptr);

    void displayImage(const QImage &image);
    void displayDetections(const QVector<QRect> &boxes);
    void updateStatistics(double precision, double recall, double f1);

private:
    ImageViewerWidget *imageViewer;
    ChartWidget *chartWidget;
    StatisticsWidget *statsWidget;
    ExportManager *exportManager;
};
