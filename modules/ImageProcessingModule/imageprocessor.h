#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

class ImageProcessor : public QQuickImageProvider
{

public:

    explicit ImageProcessor();

    //  Метод, к-рый вызывает QML при изменении sourse у Image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;





};

#endif // IMAGEPROCESSOR_H
