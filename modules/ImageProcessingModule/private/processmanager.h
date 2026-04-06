#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H


#include <QObject>
#include <QString>
#include <QSharedPointer>

#include "imagepreprocessing.h"


//class ImagePreProcessing;
class IImageProcessingService;

class ProcessManager : public QObject
{

    Q_OBJECT


public:

    explicit ProcessManager(IImageProcessingService *imageProcessingService, QObject *parent = nullptr);


    void setImagePreProcessing(ImagePreProcessing *preProcessing);


    //  Geter
    ImagePreProcessing* imagePreProcessing() const { return m_imagePreProcessing; }


public slots:

    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequestedFromFacade(const QString &filePath);


signals:

    //  To Facade for QML about Start
    void preProcessingStartNotification(bool);


    //  To ImagePreProcessing for Start
    void ImagePreProcessingRequested(const QString &filePath);


private:

    //  Создает объект ImagePreProcessing и управляет его жизненным циклом
    void createPreProcessingObject();
//const QString &filePath

    //  Удаляет объект ImagePreProcessing
    void deletePreProcessingObject();


    //  Метод использования методов обработки (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessingObject(ImagePreProcessing *ImagePreProcessing);


    IImageProcessingService *m_imageProcessingService = nullptr;


    //  Используем QPointer для обнуления, если объект удален.
    //QPointer<ImagePreProcessing> m_imagePreProcessing = nullptr;
    ImagePreProcessing *m_imagePreProcessing = nullptr;

};

#endif // PROCESSMANAGER_H
