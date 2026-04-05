#include <QDebug>

#include "processmanager.h"
#include "IImageProcessingService.h"
//#include "imagepreprocessing.h"


ProcessManager::ProcessManager(IImageProcessingService *imageProcessingService, QObject *parent)
    : QObject(parent),
    m_imageProcessingService(imageProcessingService)
{

    qDebug() << "ProcessManager: объект processManager рождён. parent: " << parent;


    connect(m_imageProcessingService, &IImageProcessingService::imagePreProcessingRequestedToProcessManager,
            this, &ProcessManager::onImagePreProcessingRequestedFromFacade);


    if (!m_imageProcessingService)
    {
        qWarning() << "ProcessManager: processManager создан без ссылки на фасад";
    }

}


void ProcessManager::setImagePreProcessing(ImagePreProcessing *imagePreProcessing)
{

    if (imagePreProcessing)
    {

        m_imagePreProcessing = imagePreProcessing;

    }   //  Добавить обработку, если imagePreProcessing = nullptr

}


ImagePreProcessing* ProcessManager::getImagePreProcessing()
{

    return m_imagePreProcessing;

}


//  Создает объект ImagePreprocessing и связывает его с фасадом
void ProcessManager::creatPreProcessingObject(const QString &filePath)
{

    //  Создает объект в куче
    ImagePreProcessing *imagePreProcessing = new ImagePreProcessing();


    setImagePreProcessing(imagePreProcessing);

    qDebug() << "ProcessManager: создал object: " << imagePreProcessing;


    //  Связь: (поменять на ProcessManager)->imagePreProcessing -> imageProcessingService
    /*connect(m_imageProcessingService, &IImageProcessingService::imagePreProcessingRequested,
            m_imagePreProcessing, &ImagePreProcessing::onImagePreProcessingRequested);*/


    usePreProcessingObject(imagePreProcessing);


    //  Уничтожает объект
    delete imagePreProcessing;

    imagePreProcessing = nullptr;

    qDebug() << "WorkingRoom: Объект ImagePreprocessing уничтожен, память очищена.";

}


//  Слушает фасад для старта предобработки
void ProcessManager::onImagePreProcessingRequestedFromFacade(const QString &filePath)
{

    qDebug() << "ProcessManager: Путь к файлу получен! Path to image: " << filePath;

}


void ProcessManager::usePreProcessingObject(ImagePreProcessing *imagePreProcessing)
{

    qDebug() << "WorkingRoom: Начинаю обработку...";


    //  Используем метод обработки
    imagePreProcessing->toGray();


    //  Сохраняем результат обработки
    cv::Mat finalMat = imagePreProcessing->getResult();

    qDebug() << "WorkingRoom: Обработка завершена. Каналов: " << finalMat.channels();

}
