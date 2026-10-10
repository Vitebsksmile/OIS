#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QImage>
#include <memory>
#include "imagepreprocessing.h"
#include "frameprocessing.h"
#include "objectfinder.h"
#include "Frame.h"
#include "yolodetectionworker.h"
#include "Core.h"
#include <QVector>

class ImageProcessingService;
//class YoloDetectionWorker;

class ProcessManager : public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(ImageProcessingService *imageProcessingService,
                            QObject *parent = nullptr);

    ~ProcessManager();

    //void setImagePreProcessing(ImagePreProcessing *preProcessing);  //???

    //  Geter
    ImagePreProcessing* imagePreProcessing() const { return m_imagePreProcessing.get(); }  //???

    void setFlag_detections(bool flag);

public slots:
    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequested(const QString &filePath);  //???

    //  ImageProcessingServise -> this
    void onProcessFrame(const cv::Mat &cvFrame);

private slots:
    //  Внутренний слот для перехвата
    //void onFrameCaptured(const cv::Mat &cvFrame);
    void onDetectionsReady(const QImage &frame,
                           const QVector<Core::Detection> &detections);

signals:
    //  this -> ImageProcessingService
    void preProcessingStartNotification(bool success);  //???

    //  this -> ImageProcessingService
    void preProcessingFinished(const QString &resultFilePath);  //???

    //  this -> ImageProcessingService
    void processedFrameReady(const QImage &frame);

    //  this -> ImageProcessingService
    void frameWithBoxesReady(const QImage &frame,
                             const std::vector<std::vector<int>> &rectanglePoints);

    // this -> VisualizationService
    void detectionsReady(const QImage &frame,
                         const QVector<Core::Detection> &detections);

private:
    //  Создает объект ImagePreProcessing и управляет его жизненным циклом
    void createPreProcessingObject();  //???

    //  Удаляет объект ImagePreProcessing
    void deletePreProcessingObject();  //???

    //  Метод использования методов обработки
    //  (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessing(ImagePreProcessing *imagePreProcessing);  //???

    QImage matToQImage(const cv::Mat &mat);
    QImage matToGrayQImage(const cv::Mat &mat);
    OIS::Core::Frame matToFrame(const cv::Mat &mat) const;  //???

    void startDetection(const cv::Mat &cvFrame);
    void stopDetection();

private:
    ImageProcessingService *m_service = nullptr;

    // Умный указатель 'unique_ptr': сам удалит объект в деструкторе или при замене
    // std::make_unique — самый безопасный способ создания объекта в куче.
    // Если m_imagePreProcessing уже владел объектом, тот удалится АВТОМАТИЧЕСКИ.
    std::unique_ptr<ImagePreProcessing> m_imagePreProcessing;  //???

    std::unique_ptr<FrameProcessing> m_processing;
    std::unique_ptr<ObjectFinder> m_finder {};

    QThread m_workerThread;
    YoloDetectionWorker *m_worker;

    bool m_flag_detections = false;
};

#endif // PROCESSMANAGER_H
