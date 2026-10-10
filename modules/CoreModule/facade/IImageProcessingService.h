//  Интерфейс модуля (публичное API)

#ifndef IIMAGEPROCESSINGSERVICE_H
#define IIMAGEPROCESSINGSERVICE_H

#include <QObject>
#include <QString>
#include <QUrl>
#include "Core.h"

namespace cv { class Mat; }

class IDatabaseService;
class ICameraManagerService;

class IImageProcessingService : public QObject
{
    Q_OBJECT

public:
    explicit IImageProcessingService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~IImageProcessingService() = default;

    virtual bool setDbService(IDatabaseService *dbService) = 0;
    virtual bool setCamService(ICameraManagerService *camService) = 0;

    virtual void readFrame(bool isRead) = 0;

public slots:
    //  VisualizationModule -> this
    virtual void onImagePreProcessingRequested(const QString &filePath) = 0;

    //  CameraManagerModule -> this
    virtual void onRawCVFrameReady(const cv::Mat &frame) = 0;

signals:
    //  For VisualizationService about Start
    void preProcessingStartNotification(bool success);

    //  Сигнал, который должен быть отправлен (emitted) после завершения работы
    //  Сообщает путь к файлу и результат (true — успех, false — провал)
    //  (for QML about Finished)
    void imagePreProcessingFinished(const QString &filePath,
                                    bool success);

    //  Сигнал для передачи конкретного текста ошибки, если что-то пошло не так,
    //  например, «файл не найден» или «недостаточно памяти»
    void prePreProcessingError(const QString &filePath,
                               const QString &error);

    //  this -> VisualizationModule
    void objectFound(const size_t &objectCount,
                     const std::vector<std::vector<int>> &rectanglePoints);

    //  this -> VisualizationModule
    void processedFrameReady(const QImage &frame);

    //  this -> VisualizationModule
    void frameWithBoxesReady(const QImage &frame,
                             const std::vector<std::vector<int>> &rectanglePoints);

    // ProcessManager -> VisualizationService
    void detectionsReady(const QImage &frame,
                         const QVector<Core::Detection> &detections);
};

//  Factory method
QSharedPointer<IImageProcessingService> createImageProcessingService(QObject* parent);

#endif // IIMAGEPROCESSINGSERVICE_H
