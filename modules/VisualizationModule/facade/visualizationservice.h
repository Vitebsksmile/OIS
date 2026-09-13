/*
 *  Сигналы (signals): Только объявляем в .h, никогда не пишем тело в .cpp.
 *  Слоты (public slots): Объявляем в .h и обязательно пишем логику в .cpp.
 *  Методы (Q_INVOKABLE): Объявляем в .h и обязательно пишем логику в .cpp.
*/


#ifndef VISUALIZATIONSERVICE_H
#define VISUALIZATIONSERVICE_H

#include <QObject>
#include <QImage>
//#include <vector>
//#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include "IVisualizationService.h"
#include "IDatabaseService.h"
#include "ICameraManagerService.h"
#include "IImageProcessingService.h"

class FileHandlerManager;
class DbModelController;
class VideoStreamService;

class VisualizationService : public IVisualizationService
{
    Q_OBJECT

public:
    explicit VisualizationService(QObject *parent = nullptr);

    bool setDbService(IDatabaseService *dbService) override;
    bool setCamService(ICameraManagerService *camService) override;
    bool setProcService(IImageProcessingService *procService) override;

//  Реализация интерфейса IVisualizationService
public slots:
    //  Слушает сигнал из FileHandler о старте предобработки
    void onImagePreProcessingRequested(const QString &filePath) override;

    //  From IMageProcessingModule for QML about Start
    void onPreProcessingStartNotification(bool success) override;

    //  в случае успеха обработки
    void onImagePreProcessingFinished(const QString &filePath, bool success) override;

    //  в случае ошибки обработки
    void onPreProcessingError(const QString &filePath, const QString &error) override;

    //  CameraManagerModule -> this
    void onImageFrameReady(const QImage frame) override;

    //  IImageProcessingModule -> this
    void onFrameReady(const QImage &frame) override;

    //  ImageProcessingModule -> this
    void onFrameWithBoxesReady(const QImage &frame
                               , const std::vector<std::vector<int>> &rectanglePoints) override;

    //  DatabaseModule -> this
    void onDefectAdded() override;

//  Мы не пишем их реализации, Qt сделает это за нас
signals:
    //  this -> VideoStreamService
    void frameReady(const QImage &frame);

private:
    IDatabaseService *m_dbService = nullptr;
    ICameraManagerService *m_camService = nullptr;
    IImageProcessingService *m_procService = nullptr;
    FileHandlerManager *m_fileHandlerManager = nullptr;
    DbModelController *m_dbController = nullptr;
    VideoStreamService *m_streamService = nullptr;
};

#endif // VISUALIZATIONSERVICE_H
