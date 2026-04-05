#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H


#include <QObject>
#include <QString>

#include "imagepreprocessing.h"


//class ImagePreProcessing;
class IImageProcessingService;

class ProcessManager : public QObject
{

    Q_OBJECT


public:

    explicit ProcessManager(IImageProcessingService *imageProcessingService, QObject *parent = nullptr);


    void setImagePreProcessing(ImagePreProcessing *imagePreProcessing);


    ImagePreProcessing* getImagePreProcessing();


    //  Создает объект ImagePreprocessing и связывает его с фасадом
    void creatPreProcessingObject(const QString &filePath);


public slots:

    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequestedFromFacade(const QString &filePath);


private:

    //  Метод использования методов обработки (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessingObject(ImagePreProcessing *ImagePreProcessing);


    IImageProcessingService *m_imageProcessingService = nullptr;
    ImagePreProcessing *m_imagePreProcessing = nullptr;

};

#endif // PROCESSMANAGER_H
