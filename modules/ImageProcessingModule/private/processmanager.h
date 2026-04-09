#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H


#include <QObject>
#include <QString>
#include <memory>

#include "imagepreprocessing.h"


class IImageProcessingService;

class ProcessManager : public QObject
{

    Q_OBJECT


public:

    explicit ProcessManager(IImageProcessingService *imageProcessingService, QObject *parent = nullptr);


    void setImagePreProcessing(ImagePreProcessing *preProcessing);


    //  Geter
    ImagePreProcessing* imagePreProcessing() const { return m_imagePreProcessing.get(); }


public slots:

    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequested(const QString &filePath);


signals:

    //  To Facade for QML about Start
    void preProcessingStartNotification(bool success);


    //  To Facade for Finished
    void preProcessingFinished(const QString &resultFilePath);


private:

    //  Создает объект ImagePreProcessing и управляет его жизненным циклом
    void createPreProcessingObject();


    //  Удаляет объект ImagePreProcessing
    void deletePreProcessingObject();


    //  Метод использования методов обработки (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessingObject(ImagePreProcessing *ImagePreProcessing);


    IImageProcessingService *m_imageProcessingService = nullptr;


    // Умный указатель 'unique_ptr': сам удалит объект в деструкторе или при замене
    // std::make_unique — самый безопасный способ создания объекта в куче.
    // Если m_imagePreProcessing уже владел объектом, тот удалится АВТОМАТИЧЕСКИ.
    std::unique_ptr<ImagePreProcessing> m_imagePreProcessing;

};

#endif // PROCESSMANAGER_H
