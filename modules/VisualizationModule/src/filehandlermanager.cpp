#include <QDebug>   //  Позволяет выводить сообщения в консоль отладки

#include "filehandlermanager.h"
//#include <filehandler.h>


FileHandlerManager::FileHandlerManager(IVisualizationService *visualizationService, QObject *parent)
    : QObject(parent),
    m_visualizationService(visualizationService)
{

    qDebug() << "FileHandlerManager: объект fileHandlerManager рождён. parent: " << parent;
    if (!m_visualizationService)
    {
        qWarning() << "FileHandlerManager создан без ссылки на фасад";
    }

}


void FileHandlerManager::registerFileHandler(FileHandler *fileHandler)
{

    if (fileHandler)
    {

        qDebug() << "FileHandlerManager: получил object: " << fileHandler;

        m_fileHandler = fileHandler;

        //  Связь: fileHandler -> visualizationService
        connect(m_fileHandler, &FileHandler::requestPreprocessingToVisualizationService,
                m_visualizationService, &IVisualizationService::PreprocessingRequestedFromTheFileHandler);


    }

}
