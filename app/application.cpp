#include <QDebug>

#include "application.h"
#include "IVisualizationService.h"
#include "visualizationservice.h"
#include "IImageProcessingService.h"
#include "imageprocessingservice.h"


Application::Application(QObject *parent)
    : QObject(parent)
{

}


Application::~Application() = default;


void Application::initialize()
{

    //  1. Создаем IVisualizationService
    m_visualizationService = QSharedPointer<IVisualizationService>(new VisualizationService());


    //  2. Создаем IImageProcessingService
    m_imageProcessingService = QSharedPointer<IImageProcessingService>(new ImageProcessingService());


    //  Настраиваем связи
    setupConnections();

}


void Application::setupConnections()
{

    //  Связываем сигнал вы

}
