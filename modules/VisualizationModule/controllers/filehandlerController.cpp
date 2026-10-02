#include "filehandlerController.h"
#include <QDebug>

FileHandlerController* FileHandlerController::s_instance = nullptr;

FileHandlerController::FileHandlerController(VisualizationService* visualization,
                                       QObject *parent)
    : QObject(parent)
    , m_visualization(visualization)
{
    s_instance = this;

    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);

    qDebug()
        << "FileHandlerManager: FileHandlerManager object created. Parent: "
        << parent;

    if (!m_visualization) {
        qWarning()
            << "WARNING! FileHandlerManager: FileHandlerManager object created without reference to facade";
    }
}

//  Регистрирует рождение объектов FileHandler и связывает их с фасадом
void FileHandlerController::registerFileHandler(FileHandler *fileHandler)
{
    if (fileHandler && !m_fileHandlers.contains(fileHandler)) {
        qDebug()
            << "FileHandlerManager: FileHandlerManager received the object: "
            << fileHandler;

        qDebug()
            << "FileHandlerManager: fileHandler.directionOut: "
            << fileHandler->directionOut();

        m_fileHandlers.append(fileHandler);

        //  Связь: fileHandler -> visualizationService
        connect(fileHandler, &FileHandler::imagePreProcessingRequested,
                m_visualization, &VisualizationService::onImagePreProcessingRequested);

        //  Связь: visualizationService -> fileHandler
        connect(m_visualization, &VisualizationService::imagePreProcessingFinished,
                fileHandler, &FileHandler::onImagePreProcessingFinished);

        /*if (fileHandler->directionOut())
        {
            //  Связь: fileHandler -> visualizationService
            connect(fileHandler, &FileHandler::imagePreProcessingRequested,
                    m_visualizationService, &VisualizationService::onImagePreProcessingRequested);
        } else {
            //  Связь: visualizationService -> fileHandler
            connect(m_visualizationService, &VisualizationService::imagePreProcessingFinished,
                    fileHandler, &FileHandler::onImagePreProcessingFinished);
        }*/
    }
}
