#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QObject>
#include <QThread>
#include <atomic>
#include <QString>
#include <QImage>
#include <memory>
#include "imagepreprocessing.h"
#include "frameprocessing.h"
#include "objectsfinder.h"
#include "Frame.h"
#include "yolodetectionworker.h"
#include "yolodetectionworker_2.h"
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

    void toggle();

public slots:
    //  Слушает фасад для старта предобработки
    void onImagePreProcessingRequested(const QString &filePath);  //???

    //  ImageProcessingServise -> this
    void onProcessFrame(const cv::Mat &cvFrame);

    void onDetectionsReady(const cv::Mat &mat,
                           const QVector<Core::Detection> &detections);

    void onDetectionFinished();

    void onWorkerError(const QString &message);

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

    void cropPCBReady(const QImage &cropPCB);

private:
    //  Создает объект ImagePreProcessing и управляет его жизненным циклом
    void createPreProcessingObject();  //???

    //  Удаляет объект ImagePreProcessing
    void deletePreProcessingObject();  //???

    //  Метод использования методов обработки
    //  (по возможности сделать принимающим разное к-во аргументов)
    void usePreProcessing(ImagePreProcessing *imagePreProcessing);  //???

    QImage matToQImage(const cv::Mat &mat);
    QImage matToQImage_2(const cv::Mat &mat);
    QImage cvMatToQImagePerfect(const cv::Mat& inMat);
    QImage matToGrayQImage(const cv::Mat &mat);
    OIS::Core::Frame matToFrame(const cv::Mat &mat) const;  //???

    //void startDetection(const cv::Mat &cvFrame);

private:
    ImageProcessingService *m_service = nullptr;

    // Умный указатель 'unique_ptr': сам удалит объект в деструкторе или при замене
    // std::make_unique — самый безопасный способ создания объекта в куче.
    // Если m_imagePreProcessing уже владел объектом, тот удалится АВТОМАТИЧЕСКИ.
    std::unique_ptr<ImagePreProcessing> m_imagePreProcessing;  //???

    std::unique_ptr<FrameProcessing> m_processing;
    std::unique_ptr<ObjectsFinder> m_finder {};

    QThread *m_workerThread;
    YoloDetectionWorker *m_worker;

    QThread *m_yoloThread = nullptr;
    YoloDetectionWorker_2 *m_yoloWorker = nullptr;
    //  true - кадр уже обрабатывается или ожидает обработки
    std::atomic_bool m_yoloBusy{false};

    std::atomic_bool m_triggered{false};
};

#endif // PROCESSMANAGER_H
