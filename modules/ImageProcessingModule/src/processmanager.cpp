#include <QDebug>

#include "processmanager.h"
#include "IImageProcessingService.h"
//#include "imagepreprocessing.h"


ProcessManager::ProcessManager(IImageProcessingService *imageProcessingService, QObject *parent)
    : QObject(parent),
    m_imageProcessingService(imageProcessingService)
{

    if (!m_imageProcessingService)
    {
        qWarning() << "ProcessManager: processManager создан без ссылки на фасад";
    }


    qDebug() << "ProcessManager: объект processManager рождён. parent: " << parent;


    //  Слушает фасад для старта предобработки
    connect(m_imageProcessingService, &IImageProcessingService::imagePreProcessingRequestedToProcessManager,
            this, &ProcessManager::onImagePreProcessingRequestedFromFacade);


    //  To Facade for QML about Start
    connect(this, &ProcessManager::preProcessingStartNotification,
            m_imageProcessingService, &IImageProcessingService::onPreProcessingStartNotification);

}


void ProcessManager::setImagePreProcessing(ImagePreProcessing *preProcessing)
{

    //  Проверка: не пытаемся ли мы установить тот же самый объект
    if (m_imagePreProcessing == preProcessing)
    {
        return;
    }


    //  Управление жизненным циклом: удаляем старый объект перед заменой
    if (m_imagePreProcessing)
    {

        //  Безопасное удаление в стиле Qt
        m_imagePreProcessing->deleteLater();

    }


    //  Устанавливаем новый объект даже если он nullptr,
    //  что позволяет «очистить текущую задачу»
    m_imagePreProcessing = preProcessing;


    if (m_imagePreProcessing)
    {
        // Гарантируем, что менеджер будет родителем (для авто-удаления)
        m_imagePreProcessing->setParent(this);
        qDebug() << "ProcessManager: Принят новый объект под управление";
    }

}


//  Создает объект ImagePreprocessing
void ProcessManager::createPreProcessingObject()
{

    //  Сначала удаляем старый объект, если он был (защита от утечки)
    if (m_imagePreProcessing)
    {
        deletePreProcessingObject();
    }


    // Передаем 'this' (ProcessManager) как родителя — это "вторая линия защиты" памяти
    ImagePreProcessing *preProcessing = new ImagePreProcessing(this);


    //  Сохраняем указатель
    setImagePreProcessing(preProcessing);


    qDebug() << "ProcessManager: создал object (preProcessing) ImagePreProcessing по адресу: " << preProcessing;
    qDebug() << "ProcessManager: создал object (imagePreProcessing()) ImagePreProcessing по адресу: " << imagePreProcessing();

}


void ProcessManager::deletePreProcessingObject()
{

    if (m_imagePreProcessing) {
        // Используем deleteLater вместо delete для безопасности в Qt
        m_imagePreProcessing->deleteLater();

        // Обнуляем указатель СРАЗУ, чтобы никто не обратился к удаленному объекту
        m_imagePreProcessing = nullptr;

        qDebug() << "ProcessManager: Объект отправлен на удаление, указатель обнулен.";
    }


    //  Уничтожает объект
    /*delete imagePreProcessing;

    imagePreProcessing() = nullptr;

    qDebug() << "WorkingRoom: Объект ImagePreprocessing уничтожен, память очищена.";*/

}


//  Слушает фасад для старта предобработки
void ProcessManager::onImagePreProcessingRequestedFromFacade(const QString &filePath)
{

    //  Создание объекта ImagePreProcessing
    createPreProcessingObject();


    //  Возвращает true/false
    //  Проверить через if()
    //imagePreProcessing()->loadImage();


    qDebug() << "ProcessManager: Путь к файлу получен! Path to image: " << filePath;


    //  Используем объект
    usePreProcessingObject(imagePreProcessing());


    //  Удаляем объект
    deletePreProcessingObject();

}


void ProcessManager::usePreProcessingObject(ImagePreProcessing *imagePreProcessing)
{

    //  To Facade for QML
    emit preProcessingStartNotification(true);


    //  To ImagePreProcessing for Start
    void ImagePreProcessingRequested(const QString &filePath);


    //  Используем метод обработки
    imagePreProcessing->toGray();


    //  Сохраняем результат обработки
    cv::Mat finalMat = imagePreProcessing->getResult();

    qDebug() << "WorkingRoom: Обработка завершена. Каналов: " << finalMat.channels();

}
