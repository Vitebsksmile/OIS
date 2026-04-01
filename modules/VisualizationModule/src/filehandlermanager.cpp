#include "filehandlermanager.h"
//#include <filehandler.h>


FileHandlerManager::FileHandlerManager(IVisualizationService *visualizationService, QObject *parent)
    : QObject(parent), m_visualizationService(visualizationService)
{

}


/*FileHandler* FileHandlerManager::createFileHandler()
{

    FileHandler *fileHandler = new FileHandler();


    if (m_visualizationService && fileHandler)
    {

        //  Менеджер сам "сватает" новый хэндлер фасаду через интерфейс
        m_visualizationService->registerFileHandler(fileHandler);

    }

    m_fileHandler = fileHandler;


    return fileHandler;

}*/


/*void FileHandlerManager::registerFileHandler(FileHandler *fileHandler)
{

    if (fileHandler)
    {

        qDebug() << "FileHandlerManager: получил FileHandler: " << fileHandler;

    }

}*/


void FileHandlerManager::printMessage(const QString &msg)
{

    if (!msg.isEmpty())
    {
        qDebug() << msg;
    }

}
