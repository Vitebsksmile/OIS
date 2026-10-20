#ifndef IVISUALIZATIONSERVICE_H
#define IVISUALIZATIONSERVICE_H

#include <QObject>
#include <QString>
#include <QImage>
#include "Core.h"

class IDatabaseService;
class ICameraManagerService;
class IImageProcessingService;

class IVisualizationService : public QObject
{
    Q_OBJECT

public:
    //  explicit — запрещает неявное приведение типов
    explicit IVisualizationService(QObject *parent = nullptr) : QObject(parent) {}

    //  Виртуальный деструктор
    //  Критически важен для интерфейсов: он гарантирует,
    //  что при удалении объекта через указатель на интерфейс
    //  будет вызван деструктор именно дочернего (реального) класса
    virtual ~IVisualizationService() = default;

    virtual bool setDbService(IDatabaseService *dbService) = 0;
    virtual bool setCamService(ICameraManagerService *camService) = 0;
    virtual bool setProcService(IImageProcessingService *procService) = 0;

public slots:
    //  IImageProcessingModule -> this
    virtual void onPreProcessingStartNotification(bool success) = 0;    //???

    //  IImageProcessingModule -> this
    virtual void onImagePreProcessingFinished(const QString &filePath,
                                              bool success) = 0;    //???

    //  IImageProcessingModule -> this
    virtual void onPreProcessingError(const QString &filePath,
                                      const QString &error) = 0;    //???

    //  CameraManagerModule -> this
    virtual void onRawImageFrameReady(const QImage &frame) = 0;

    //  IImageProcessingModule -> this
    virtual void onProcessedFrameReady(const QImage &frame) = 0;

    //  ImageProcessingModule -> this
    virtual void onFrameWithBoxesReady(const QImage &frame,
                                       const std::vector<std::vector<int>> &rectanglePoints) = 0;

    // ProcessManager -> VisualizationService
    virtual void onDetectionsReady(const QImage &frame,
                                   const QVector<Core::Detection> &detections) = 0;

    virtual void onCropPCBReady(const QImage &cropPCB) = 0;    //???

    //virtual void onDbExecutionError(const QString &error) = 0;

signals:
    //  this -> ImageProcessingModule
    //  Вызываем его через emit, когда в интерфейс приходит команда начать PreProcessing
    void imagePreProcessingRequested(const QString &filePath);    //???

    void triggered();
};

//  Factory method
QSharedPointer<IVisualizationService> createVisualizationService(QObject* parent);

#endif // IVISUALIZATIONSERVICE_H
