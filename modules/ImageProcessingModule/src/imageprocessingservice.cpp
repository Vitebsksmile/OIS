/*
 * Этот файл реализует асинхронную логику:
 * здесь Qt (управление потоками) встречается с OpenCV (математика изображений).
 * Главная фишка кода в том, что интерфейс не «зависает» во время обработки.
*/
/*
 * Почему это написано грамотно:
 * Thread Safety: Сигнал finished у QFutureWatcher всегда приходит в тот поток, где был создан объект (обычно в главный UI-поток). Это позволяет безопасно обновлять интерфейс.
 * Лямбда-захват: В QtConcurrent::run передается filePath по значению, что безопасно для многопоточности.
 * Инкапсуляция: Сложная математика (performProcessing) отделена от логики загрузки и потоков.
*/


#include <QDebug>
#include <QThread>

#include "imageprocessingservice.h"
#include "imagepreprocessing.h"
#include "processmanager.h"


ImageProcessingService::ImageProcessingService(QObject *parent)
    : IImageProcessingService(parent),
    m_processManager(new ProcessManager (this, this))
{

}


//  Слот для получения пути из VisualizationModule
//  Запуск обработки
void ImageProcessingService::onImagePreProcessingRequested(const QString &filePath)
{

    //  Базовая проверка: если путь пустой, то сразу выходим с ошибкой
    if (filePath.isEmpty())
    {
        qDebug() << "ImageProcessingService: The file path is empty! Path to image: " << filePath;
        emit prePreProcessingError(filePath, "Empty file path");
        return;
    }

    //  Запоминаем путь, чтобы потом передать его в сигнале завершения
    m_currentFilePath = filePath;
    qDebug() << "ImageProcessingService: The file path has been obtained! Path to image: " << filePath << ". I pass it to ProcessManager.";
    emit imagePreProcessingRequested(filePath);

}


//  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
//  для уведомления User о начале предобработки (for QML about Start)
void ImageProcessingService::onPreProcessingStartNotification(bool success) {}


//  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
//  для уведомления о завершении предобработки (for QML about Finished)
void ImageProcessingService::onPreProcessingFinished(const QString &resultFilePath)
{

    imagePreProcessingFinished("file:///" + resultFilePath, true);
    qDebug() << "ImageProcessingService: Preprocessing completion signal sent; resultFilePath: " << resultFilePath;

}
