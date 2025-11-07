#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class ImageViewerWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewerWidget(QWidget *parent = nullptr);
    void displayImage(const QImage &image);
    void drawDetections(const QVector<QRect> &boxes);

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pixmapItem;
};
