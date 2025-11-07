#include "imageviewerwidget.h"
#include <QGraphicsRectItem>
#include <QPen>

ImageViewerWidget::ImageViewerWidget(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this))
{
    setScene(scene);
    pixmapItem = scene->addPixmap(QPixmap());
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageViewerWidget::displayImage(const QImage &image)
{
    scene->clear();
    pixmapItem = scene->addPixmap(QPixmap::fromImage(image));
    scene->setSceneRect(image.rect());
}

void ImageViewerWidget::drawDetections(const QVector<QRect> &boxes)
{
    QPen pen(Qt::red);
    pen.setWidth(2);
    for (const QRect &box : boxes)
        scene->addRect(box, pen);
}
