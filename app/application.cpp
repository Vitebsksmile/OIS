#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "IDatabaseService.h"
#include "IImageProcessingService.h"
#include "ICameraManagerService.h"


Application::Application(int &argc, char **argv, QObject *parent)
    : QObject(parent)
    , m_app(new QGuiApplication(argc, argv))
{
    qDebug() << qEnvironmentVariable("PATH");
    qDebug() << "Application.cpp: Created QGuiApplication";
}

Application::~Application()
{
    //  Очистка в обратном порядке
    m_engine.reset();
}

bool Application::initialize()
{
    qInfo() << "Application: Initializing Application...";

    //  Create facade database module
    m_dbService = createDatabaseService(this);
    if (m_dbService) {
        qDebug() << "Application: DatabaseService initialized";
    } else {
        qCritical() << "WARNING! Application: Failed to create DatabaseService";
    }

    //  Создаем фасад модуля визуализации
    m_visualizationService = createVisualizationService(this);
    if (m_visualizationService) {
        qDebug() << "Application: VisualizationService initialized";
    } else {
        qCritical() << "WARNING! Application: Failed to create VisualizationService";
        return false;
    }

    //  Создаем фасад модуля обработки изображений
    m_imageProcessingService = createImageProcessingService(this);
    if (m_imageProcessingService) {
        qDebug() << "Application: ImageProcessingService initialized";
    } else {
        qCritical() << "WARNING! Application: Failed to create ImageProcessingService";
        return false;
    }

    //  Создаем фасад модуля камеры
    m_cameraManagerService = createCameraManagerService(this);
    if (m_cameraManagerService) {
        qDebug() << "Application: CameraManagerService initialized";
    } else {
        qCritical() << "WARNING! Application: Failed to create CameraManagerService";
        return false;
    }

    setupConnections();

    qDebug() << "Application: Application initialized successfully.";

    return true;
}

bool Application::modulesIntegration()
{
    bool flag = false;

    if (m_visualizationService->setDbService(m_dbService.get())) {
        qDebug() << "Application: DatabaseService object has been successfuly passed to VisualizationService";
        flag = true;
    } else {
        qCritical() << "WARNING! Application: Passing the DatabaseService object to VisualizationService failed!";
        flag = false;
    }

    if (m_visualizationService->setProcService(m_imageProcessingService.get())) {
        qDebug() << "Application: ImageProcessingService object has been successfuly passed to VisualizationService";
        flag = true;
    } else {
        qCritical() << "WARNING! Application: Passing the ImageProcessingService object to VisualizationService failed!";
        flag = false;
    }

    if (m_imageProcessingService->setDbService(m_dbService.get())) {
        qDebug() << "Application: DatabaseService object has been successfuly passed to ImageProcessingService";
        flag = true;
    } else {
        qCritical() << "WARNING! Application: Passing the DatabaseService object to ImageProcessingService failed!";
        flag = false;
    }

    if (flag) {
        return true;
    } else {
        return false;
    }
}

void Application::setupConnections()
{
    //  Связь: VisualizationModule -> ImageProcessingModule
    bool ok = connect(m_visualizationService.get(), &IVisualizationService::imagePreProcessingRequested,
                      m_imageProcessingService.get(), &IImageProcessingService::onImagePreProcessingRequested);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between VisualizationModule -> ImageProcessingModule";

    //  Связи: ImageProcessingModule -> VisualizationModule
    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::preProcessingStartNotification,
                 m_visualizationService.get(), &IVisualizationService::onPreProcessingStartNotification);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between ImageProcessingModule -> VisualizationModule";

    //  Связь: ImageProcessingModule -> VisualizationModule
    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::imagePreProcessingFinished,
                 m_visualizationService.get(), &IVisualizationService::onImagePreProcessingFinished);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between ImageProcessingModule -> VisualizationModule";

    //  Связь: ImageProcessingModule -> VisualizationModule
    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::prePreProcessingError,
                 m_visualizationService.get(), &IVisualizationService::onPreProcessingError);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between ImageProcessingModule -> VisualizationModule";

    //  Связь: CameraManagerModule -> VisualizationModule
    ok = connect(m_cameraManagerService.get(), &ICameraManagerService::imageFrameReady,
                 m_visualizationService.get(), &IVisualizationService::onImageFrameReady);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between CameraManagerModule -> VisualizationModule";

    //  CameraManagerModule -> ImageProcessingModule
    ok = connect(m_cameraManagerService.get(), &ICameraManagerService::cvFrameReady
                 , m_imageProcessingService.get(), &IImageProcessingService::onCVFrameReady);
    if (!ok) qCritical() << "WARNING! Application: Failed to establish connection between CameraManagerModule -> ImageProcessingModule";
}


int Application::run()
{
    qDebug() << "Application: Starting Application...";

    m_engine.reset(new QQmlApplicationEngine());

    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");

    m_engine->load(url);  //   загружаем интерфейс

    if (m_engine->rootObjects().isEmpty())
    {
        qCritical() << "WARNING! Application: Failed to load QML file";

        return -1;
    }

    return m_app->exec();
}

/*QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);*/
