#ifndef IMAGEPREPROCESSING_H
#define IMAGEPREPROCESSING_H

#include <QQuickImageProvider>
#include <opencv2/opencv.hpp>

class ImagePreProcessing : public QQuickImageProvider
{

public:

    explicit ImagePreProcessing();

    //  Метод, к-рый вызывает QML при изменении sourse у Image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;





};

#endif // IMAGEPREPROCESSING_H
