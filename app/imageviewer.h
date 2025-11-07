#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <opencv2/cvv/cvv.hpp>

class ImageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);

//public slots:

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *imageItem;

    void setupScene();
    QPixmap mapToPixmap(const cv::Mat &mat);
};

#endif // IMAGEVIEWER_H
