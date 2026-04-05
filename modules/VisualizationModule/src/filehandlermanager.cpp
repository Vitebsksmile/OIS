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
        qWarning() << "FileHandlerManager: fileHandlerManager создан без ссылки на фасад";
    }

}


//  Регистрирует рождение объекта FileHandler и связывает его с фасадом
void FileHandlerManager::registerFileHandler(FileHandler *fileHandler)
{

    if (fileHandler)
    {

        qDebug() << "FileHandlerManager: получил object: " << fileHandler;

        m_fileHandler = fileHandler;

        //  Связь: fileHandler -> visualizationService
        connect(m_fileHandler, &FileHandler::imagePreProcessingRequested,
                m_visualizationService, &IVisualizationService::onImagePreProcessingRequested);


    }

}
