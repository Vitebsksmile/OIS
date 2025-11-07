#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(QWidget *parent)
    : QWidget(parent)
{
    precisionLabel = new QLabel("Precision: 0.0", this);
    recallLabel = new QLabel("Recall: 0.0", this);
    f1Label = new QLabel("F1-Score: 0.0", this);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(precisionLabel);
    layout->addWidget(recallLabel);
    layout->addWidget(f1Label);
    setLayout(layout);
}

void StatisticsWidget::setMetrics(double precision, double recall, double f1)
{
    precisionLabel->setText(QString("Precision: %1").arg(precision, 0, 'f', 3));
    recallLabel->setText(QString("Recall: %1").arg(recall, 0, 'f', 3));
    f1Label->setText(QString("F1-Score: %1").arg(f1, 0, 'f', 3));
}
