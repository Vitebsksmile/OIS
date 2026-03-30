#include "filehandlermanager.h"
#include <filehandler.h>


FileHandlerManager::FileHandlerManager(IVisualizationService *service, QObject *parent)
    : QObject(parent), m_service(service)
{

}


FileHandler* FileHandlerManager::createFileHandler()
{

    FileHandler *fileHandler = new FileHandler();


    if (m_service)
    {

        //  Менеджер сам "сватает" новый хэндлер фасаду через интерфейс
        m_service->registerFileHandler(fileHandler);

    }


    return fileHandler;

}
