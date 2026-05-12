#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "visualizationService.h"
#include "IImageProcessingService.h"
#include "imageprocessingservice.h"


Application::Application(int &argc, char **argv, QObject *parent)
    : QObject(parent)
    , m_app(new QGuiApplication(argc, argv))
{
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


    setupConnections();


    qDebug() << "Application: Application initialized successfully.";

    return true;

}


void Application::setupConnections()
{

    //  Связь: VisualizationModule -> ImageProcessingModule
    bool ok = connect(m_visualizationService.get(), &IVisualizationService::imagePreProcessingRequested,
            m_imageProcessingService.get(), &IImageProcessingService::onImagePreProcessingRequested);

    if (!ok) qDebug() << "Failed to establish connection between VisualizationModule -> ImageProcessingModule";


    //  Связи: ImageProcessingModule -> VisualizationModule
    connect(m_imageProcessingService.get(), &IImageProcessingService::preProcessingStartNotification,
            m_visualizationService.get(), &IVisualizationService::onPreProcessingStartNotification);

    if (!ok) qDebug() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";


    connect(m_imageProcessingService.get(), &IImageProcessingService::imagePreProcessingFinished,
            m_visualizationService.get(), &IVisualizationService::onImagePreProcessingFinished);

    if (!ok) qDebug() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";


    connect(m_imageProcessingService.get(), &IImageProcessingService::prePreProcessingError,
            m_visualizationService.get(), &IVisualizationService::onPreProcessingError);

    if (!ok) qDebug() << "Failed to establish connection between ImageProcessingModule -> VisualizationModule";

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
