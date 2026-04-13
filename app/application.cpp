#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "visualizationService.h"
#include "IImageProcessingService.h"
#include "imageprocessingservice.h"


/*Application::Application(QObject *parent)
    : QObject(parent)
{
}*/
Application::Application(int &argc, char **argv, QObject *parent)
    : QObject(parent)
    , m_app(new QGuiApplication(argc, argv))
{
    qDebug() << "Application.cpp: Создали QGuiApplication";
}


Application::~Application()
{

    //  Очистка в обратном порядке
    m_engine.reset();

    //???m_app.reset();

}


bool Application::initialize()
{

    qDebug() << "Application: Initializing Application...";


    //  1. Создаем фасад модуля визуализации
    m_visualizationService = QSharedPointer<IVisualizationService>(new VisualizationService(this));


    if (m_visualizationService)
    {

        qDebug() << "Application: VisualizationService инициализирован";

    } else {

        qCritical() << "Failed to create VisualizationService";

        return false;

    }


    //  2. Создаем фасад модуля обработки изображений
    m_imageProcessingService = QSharedPointer<IImageProcessingService>(new ImageProcessingService(this));


    if (m_imageProcessingService)
    {

        qDebug() << "Application: ImageProcessingService инициализирован";

    } else {

        qCritical() << "Failed to create ImageProcessingService";

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

    if (!ok) qDebug() << "Сбой наладки связи VisualizationModule -> ImageProcessingModule";


    //  Связи: ImageProcessingModule -> VisualizationModule
    connect(m_imageProcessingService.get(), &IImageProcessingService::imagePreProcessed,
            m_visualizationService.get(), &IVisualizationService::onImageProcessed);

    if (!ok) qDebug() << "Сбой наладки связи ImageProcessingModule -> VisualizationModule";


    connect(m_imageProcessingService.get(), &IImageProcessingService::preProcessingError,
            m_visualizationService.get(), &IVisualizationService::onProcessingError);

    if (!ok) qDebug() << "Сбой наладки связи ImageProcessingModule -> VisualizationModule";

}


//new   int Application::run(int argc, char *argv[])
//new
int Application::run()
{

    qDebug() << "Starting Application...";


    //new  2. Создаем Qml engine
    m_engine.reset(new QQmlApplicationEngine());


    //new  4. Загружаем главный Qml файл
    //  Запускаем не из физического расположения файла, а из логического расположения (см. CMAKE VisualizationModule)
    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");

    //new
    m_engine->load(url);  //   загружаем интерфейс

    //new
    if (m_engine->rootObjects().isEmpty())
    {
        qCritical() << "Failed to load QML file";
        return -1;
    }


    //  1. Сохраняем argc в член класса, чтобы ссылка была валидна все время жизни a_app
    //new   m_argc = argc;


    //  1. Создаем Qt приложение
    //new   m_app.reset(new QGuiApplication(argc, argv));
    //qDebug() << "Application.cpp: Создали QGuiApplication";

/*new
    //  2. Создаем Qml engine
    m_engine.reset(new QQmlApplicationEngine());


    //  4. Загружаем главный Qml файл
    //  Запускаем не из физического расположения файла, а из логического расположения (см. CMAKE VisualizationModule)
    const QUrl url("qrc:/qt/qml/VisualizationModule/Visualization.qml");


    m_engine->load(url);  //   загружаем интерфейс


    if (m_engine->rootObjects().isEmpty())
    {
        qCritical() << "Failed to load QML file";
        return -1;
    }
*/

    return m_app->exec();

}

/*QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);*/
