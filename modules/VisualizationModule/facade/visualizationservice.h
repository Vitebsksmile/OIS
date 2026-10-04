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

class AuthController;
class SessionController;
class FileHandlerController;
class DbModelController;
class VideoStreamController;

class VisualizationService : public IVisualizationService
{
    Q_OBJECT

public:
    explicit VisualizationService(QObject *parent = nullptr);

    bool setDbService(IDatabaseService *dbService) override;
    bool setCamService(ICameraManagerService *camService) override;
    bool setProcService(IImageProcessingService *procService) override;

    DbModelController* dbController();

    IDatabaseService* dbService();

    //IDbModel* dbModel()

    bool creatCamera(const QString &url);

//  Реализация интерфейса IVisualizationService
public slots:
    //  FileHandler -> this : Слушает сигнал из FileHandler о старте предобработки
    void onImagePreProcessingRequested(const QString &filePath);

    //  From IMageProcessingModule for QML about Start
    void onPreProcessingStartNotification(bool success) override;

    //  в случае успеха обработки
    void onImagePreProcessingFinished(const QString &filePath, bool success) override;

    //  в случае ошибки обработки
    void onPreProcessingError(const QString &filePath, const QString &error) override;

    //  CameraManagerModule -> this
    void onRawImageFrameReady(const QImage &frame) override;

    //  IImageProcessingModule -> this
    void onProcessedFrameReady(const QImage &frame) override;

    //  ImageProcessingModule -> this
    void onFrameWithBoxesReady(const QImage &frame
                               , const std::vector<std::vector<int>> &rectanglePoints) override;

    //  DatabaseModule -> this
    void onDefectAdded() override;

    //void onDbExecutionError(const QString &error) override;

//  Мы не пишем их реализации, Qt сделает это за нас
signals:
    //  To FileHandler about started PreProcessing
    void preProcessingStartNotification(bool success);

    //  To FileHandler about finished
    void imagePreProcessingFinished(const QString &filePath);

    //  this -> VideoStreamController
    void rawImageFrameReady(const QImage &frame);

    //  this -> VideoStreamService
    void processedFrameReady(const QImage &frame);

    //  this -> VideoStreamService
    void frameWithBoxesReady(const QImage &frame,
                             const std::vector<std::vector<int>> &rectanglePoints);

private:
    IDatabaseService *m_dbService = nullptr;
    ICameraManagerService *m_camService = nullptr;
    IImageProcessingService *m_procService = nullptr;

    AuthController *m_authController = nullptr;
    SessionController *m_sessController = nullptr;
    DbModelController *m_dbController = nullptr;
    FileHandlerController *m_fileHandlerController = nullptr;
    VideoStreamController *m_videoController = nullptr;
};

#endif // VISUALIZATIONSERVICE_H
