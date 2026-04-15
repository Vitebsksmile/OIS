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


//  Регистрирует рождение объектов FileHandler и связывает их с фасадом
void FileHandlerManager::registerFileHandler(FileHandler *fileHandler)
{

    if (fileHandler && !m_fileHandlers.contains(fileHandler))
    {

        qDebug() << "FileHandlerManager: получил object: " << fileHandler;
        qDebug() << "FileHandlerManager: fileHandler.directionOut: " << fileHandler->directionOut();

        m_fileHandlers.append(fileHandler);


        if (fileHandler->directionOut())
        {

            //  Связь: fileHandler -> visualizationService
            connect(fileHandler, &FileHandler::imagePreProcessingRequested,
                    m_visualizationService, &IVisualizationService::onImagePreProcessingRequested);

        } else {

            //  Связь: visualizationService -> fileHandler
            connect(m_visualizationService, &IVisualizationService::imagePreProcessingFinished,
                    fileHandler, &FileHandler::onImagePreProcessingFinished);

        }

    }

}
