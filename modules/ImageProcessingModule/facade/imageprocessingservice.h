/*
 * Этот файл описывает конкретную реализацию процессора изображений,
 * которая использует библиотеку OpenCV для расчетов и QtConcurrent для того,
 * чтобы тяжелая обработка не «вешала» интерфейс.
*/

/*
 * Почему это правильный подход:
 * Разделение ответственности: Метод performProcessing ничего не знает о Qt и путях — он просто крутит пиксели.
 * Многопоточность: Благодаря QtConcurrent и QFutureWatcher, тяжелая математика OpenCV уходит в другой поток.
 * Пользователь может продолжать нажимать кнопки в SideBar, пока фото обрабатывается.
 * Безопасность: Использование watcher позволяет избежать проблем с синхронизацией потоков (вам не нужны мьютексы в явном виде).
*/

#ifndef IMAGEPROCESSINGSERVICE_H
#define IMAGEPROCESSINGSERVICE_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "IImageProcessingService.h"
#include "IDatabaseService.h"
#include "ICameraManagerService.h"

class ProcessManager;
class OnnxDefectDetector;

class ImageProcessingService : public IImageProcessingService
{
    Q_OBJECT

public:
    explicit ImageProcessingService(QObject* parent = nullptr);

    bool setDbService(IDatabaseService *dbService) override;
    bool setCamService(ICameraManagerService *camService) override;

    void changesDetectionMethod(const QString &detectionMethod) override;

//  Реализация интерфейса IImageProcessingService
public slots:
    //  Слот для получения пути из VisualizationModule
    void onImagePreProcessingRequested(const QString &filePath) override;  //???

    //  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
    //  для уведомления User о начале предобработки (for QML about Start)
    void onPreProcessingStartNotification(bool success);  //???

    //  ProcessManager -> this
    //  для уведомления о завершении предобработки (for QML about Finished)
    void onPreProcessingFinished(const QString &resultFilePath);  //???

    //  CameraManagerService -> this
    void onRawCVFrameReady(const cv::Mat &cvFrame) override;

    //  ProcessManager -> this
    void onProcessedFrameReady(const QImage &frame);

    //  ProcessManager -> this
    void onFrameWithBoxesReady(const QImage &frame,
                               const std::vector<std::vector<int>> &rectanglePoints);

signals:
    //  Сигнал для ProcessManager -> создай imagePreProcessing
    void imagePreProcessingRequested(const QString &filePath);  //???

    //  this -> ProcessManager -> for creat onnxDefectDetector
    void defectDetectionRequested();

    //  this -> ProcessManager
    void processFrame(const cv::Mat &cvFrame, const QString &detectionMethod);
    //void processedFrameReady(ProcessedFrame frame);  //???

private:
    ProcessManager* m_processManager;

    //  Хранит путь к файлу, который обрабатывается в данный момент,
    //  чтобы знать, какой путь отправить обратно в сигнале imageProcessed
    QUrl m_currentFilePath;  //???
    IDatabaseService *m_dbService = nullptr;
    ICameraManagerService *m_camService = nullptr;
    QString m_detectionMethod  = "yolo11";
};

#endif // IMAGEPROCESSINGSERVICE_H
