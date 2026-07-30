#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "visualizationService.h"
#include "IImageProcessingService.h"
#include "imageprocessingservice.h"
#include "ICameraManagerService.h"
#include "cameramanagerservice.h"


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

    qDebug() << "Application: Initializing Application...";


    //  1. Создаем фасад модуля визуализации
    m_visualizationService = QSharedPointer<IVisualizationService>(new VisualizationService(this));


    if (m_visualizationService)
    {

        qDebug() << "Application: VisualizationService initialized";

    } else {

        qCritical() << "Application: Failed to create VisualizationService";

        return false;

    }


    //  2. Создаем фасад модуля обработки изображений
    m_imageProcessingService = QSharedPointer<IImageProcessingService>(new ImageProcessingService(this));


    if (m_imageProcessingService)
    {

        qDebug() << "Application: ImageProcessingService initialized";

    } else {

        qCritical() << "Application: Failed to create ImageProcessingService";

        return false;

    }

    //  3. Создаем фасад модуля камеры
    m_cameraManagerService = QSharedPointer<ICameraManagerService>(new CameraManagerService(this));

    if (m_cameraManagerService)
    {
        qDebug() << "Application: CameraManagerService initialized";

    } else {

        qCritical() << "Application: Failed to create CameraManagerService";

        return false;
    }


    setupConnections();


    qDebug() << "Application: Application initialized successfully.";

    return true;

}


void Application::setupConnections()
{

    //  Связь: VisualizationModule -> ImageProcessingModule
    bool ok = connect(m_visualizationService.get(), &IVisualizationService::imagePreProcessingRequested,
                      m_imageProcessingService.get(), &IImageProcessingService::onImagePreProcessingRequested);

    if (!ok) qCritical() << "Failed to establish connection between VisualizationModule -> ImageProcessingModule";


    //  Связи: ImageProcessingModule -> VisualizationModule
    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::preProcessingStartNotification,
                 m_visualizationService.get(), &IVisualizationService::onPreProcessingStartNotification);

    if (!ok) qCritical() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";


    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::imagePreProcessingFinished,
                 m_visualizationService.get(), &IVisualizationService::onImagePreProcessingFinished);

    if (!ok) qCritical() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";


    ok = connect(m_imageProcessingService.get(), &IImageProcessingService::prePreProcessingError,
                 m_visualizationService.get(), &IVisualizationService::onPreProcessingError);

    if (!ok) qCritical() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";

    ok = connect(m_cameraManagerService.get(), &ICameraManagerService::frameReady,
                 m_visualizationService.get(), &IVisualizationService::onFrameReady);

    if (!ok) qCritical() << "Failed to establish connection between CameraManagerModule -> VisualizationModule";

    ok = connect(m_visualizationService.get(), &IVisualizationService::startReady,
                 m_cameraManagerService.get(), &ICameraManagerService::onStartReady);

    if (!ok) qCritical() << "Failed to establish connection between CameraManagerModule -> VisualizationModule";

}


int Application::run()
{

    qDebug() << "Application: Starting Application...";


    m_engine.reset(new QQmlApplicationEngine());


    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");

    m_engine->load(url);  //   загружаем интерфейс

    if (m_engine->rootObjects().isEmpty())
    {
        qCritical() << "Failed to load QML file";

        return -1;
    }


    return m_app->exec();

}

/*QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);*/
