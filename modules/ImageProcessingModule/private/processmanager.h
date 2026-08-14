#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <memory>
#include "imagepreprocessing.h"
#include "frameprocessing.h"
#include "objectfinder.h"

class IImageProcessingService;

class ProcessManager : public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(
        IImageProcessingService *imageProcessingService,
        QObject *parent = nullptr);

    void setImagePreProcessing(ImagePreProcessing *preProcessing);

    //  Geter
    ImagePreProcessing* imagePreProcessing() const { return m_imagePreProcessing.get(); }

public slots:
    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequested(const QString &filePath);

    //  ImageProcessingServise -> this
    void onProcessFrame(const cv::Mat &cvFrame);

signals:
    //  this -> ImageProcessingService
    void preProcessingStartNotification(bool success);

    //  this -> ImageProcessingService
    void preProcessingFinished(const QString &resultFilePath);\

    //  this -> ImageProcessingService
    void frameWithBoxesReady(const QImage &frame
                             , const std::vector<std::vector<int>> &rectanglePoints);

private:
    //  Создает объект ImagePreProcessing и управляет его жизненным циклом
    void createPreProcessingObject();

    //  Удаляет объект ImagePreProcessing
    void deletePreProcessingObject();

    //  Метод использования методов обработки
    //  (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessing(ImagePreProcessing *imagePreProcessing);

    void useFrameProcessing(FrameProcessing *processing);
    void useFinder(ObjectFinder *finder);

    QImage matToQImage(const cv::Mat &mat);

private:
    IImageProcessingService *m_imageProcessingService = nullptr;

    // Умный указатель 'unique_ptr': сам удалит объект в деструкторе или при замене
    // std::make_unique — самый безопасный способ создания объекта в куче.
    // Если m_imagePreProcessing уже владел объектом, тот удалится АВТОМАТИЧЕСКИ.
    std::unique_ptr<ImagePreProcessing> m_imagePreProcessing;

    std::unique_ptr<FrameProcessing> m_processing;
    std::unique_ptr<ObjectFinder> m_finder;
};

#endif // PROCESSMANAGER_H
