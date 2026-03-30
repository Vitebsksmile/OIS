#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "visualizationService.h"
#include "IImageProcessingService.h"
#include "imageprocessingservice.h"



Application::Application(QObject *parent)
    : QObject(parent)
{
}


Application::~Application()
{

    //  Очистка в обратном порядке
    m_engine.reset();

    m_app.reset();

}


bool Application::initialize()
{

    qDebug() << "Initializing Application...";


    //  1. Создаем модуль визуализации
    m_visualizationService = QSharedPointer<IVisualizationService>(new VisualizationService(this));


    if (!m_visualizationService)
    {

        qCritical() << "Failed to create VisualizationService";

        return false;

    }


    //  2. Создаем модуль обработки изображений
    m_imageProcessingService = QSharedPointer<IImageProcessingService>(new ImageProcessingService(this));


    if (!m_imageProcessingService)
    {

        qCritical() << "Failed to create ImageProcessingService";

        return false;

    }


    qDebug() << "Application initialized successfully";

    return true;

}


void Application::setupConnections()
{

    //  Связь: VisualizationModule -> ImageProcessingModule
    connect(m_visualizationService.data(), &IVisualizationService::requestPreprocessing,
            m_imageProcessingService.data(), &IImageProcessingService::onPreprocessingRequested);


    //  Связи: ImageProcessingModule -> VisualizationModule
    connect(m_imageProcessingService.data(), &IImageProcessingService::imageProcessed,
            m_visualizationService.data(), &IVisualizationService::onImageProcessed);


    connect(m_imageProcessingService.data(), &IImageProcessingService::processingError,
            m_visualizationService.data(), &IVisualizationService::onProcessingError);

}


int Application::run(int argc, char *argv[])
{

    qDebug() << "Starting Application...";


    //  1. Сохраняем argc в член класса, чтобы ссылка была валидна все время жизни a_app
    m_argc = argc;


    //  1. Создаем Qt приложение
    m_app.reset(new QGuiApplication(argc, argv));


    //  2. Создаем Qml engine
    m_engine.reset(new QQmlApplicationEngine());


    //  4. Загружаем главный Qml файл
    //  Запускаем не из физического расположения файла, а из логического расположения (см. CMAKE VisualizationModule)
    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");


    m_engine->load(url);  //   загружаем интерфейс


    /*QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);*/


    if (m_engine->rootObjects().isEmpty())
    {
        qCritical() << "Failed to load QML file";
        return -1;
    }


    return m_app->exec();

}
