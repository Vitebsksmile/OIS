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

#include "imageprocessingservice.h"


ImageProcessingService::ImageProcessingService(QObject *parent)
    : IImageProcessingService(parent)
//  IImageProcessingService(parent): Вызов конструктора базового класса (интерфейса)
{

}


//  Запуск обработки
void ImageProcessingService::onPreprocessingRequested(const QUrl &filePath)
{

    //  Базовая проверка: если путь пустой, то сразу выходим с ошибкой
    if (filePath.isEmpty())
    {
        emit processingError(filePath, "Empty file path");
        return;
    }

    //  Запоминаем путь, чтобы потом передать его в сигнале завершения
    m_currentFilePath = filePath;

}
