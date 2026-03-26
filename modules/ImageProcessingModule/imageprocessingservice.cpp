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
{

    //  Создаем "наблюдателя", к-рый будет следить за фоновым потоком
    //  Тип <cv::Mat> означает, что мы ждем матрицу OpenCV на выходе
    m_watcher = new QFutureWatcher<cv::Mat>(this);


    //  Лямбда-функция, к-рая сработает автоматически, когда фоновый поток закончит работу
    connect(m_watcher,
            &QFutureWatcher<cv::Mat>::finished,
            [this]()
            {
                //  Проверка: если задачу отменили, то ни чего не делаем
                if (m_watcher->isCanceled()) return;

                //  Забираем результат вычислений из потока
                cv::Mat result = m_watcher->result();

                //  Емли матрица не пуста,
                if (!result.empty())
                {
                    //  ..то сигналим об успехе
                    emit imageProcessed(m_currentFilePath, true);
                } else {
                    //  Иначе сообщаем об ошибке обработки
                    emit processingError(m_currentFilePath, "Processing failed");
                }

            });

}


//  Запуск обработки
void ImageProcessingService::processImage(const QUrl &filePath)
{

    //  Базовая проверка: если путь пустой, то сразу выходим с ошибкой
    if (filePath.isEmpty())
    {
        emit processingError(filePath, "Empty file path");
        return;
    }

    //  Запоминаем путь, чтобы потом передать его в сигнале завершения
    m_currentFilePath = filePath;

    //  Асинхронная обработка
    //  Запускаем блок кода в отдельном потоке из пула Qt
    QFuture<cv::Mat> future = QtConcurrent::run([this, filePath]() -> cv::Mat {

        //  Загружаем картинку средствами OpenCV
        //  путь конвертируем из QUrl в std::string
        cv::Mat img = cv::imread(filePath.toString().toStdString());

        //  Если файл не открылся (не картинка или нет прав),
        if (img.empty())
        {
            //  ..то сигналим об ошибке
            emit processingError(filePath, "Failed to load image");

            //  и возвращаем пустую матрицу
            return cv::Mat();
        }

        //  Если загрузка успешна, то передаем матрицу в метод с алгоритмами
        return performProcessing(img);

    });

    //  "Привязываем" нашего наблюдателя к запущенной задаче
    //  Теперь он знает за чем именно следить
    m_watcher->setFuture(future);

}


//  Алгоритм OpenCV
cv::Mat ImageProcessingService::performProcessing(const cv::Mat &input)
{

    //  Результирующая матрица
    cv::Mat result;

    //  Преобразование цветного изображения (BRG) в градации серого (GRAY)
    cv::cvtColor(input, result, cv::COLOR_BGR2GRAY);

    //  Возвращаем готовую картинку обратно в поток
    return result;

}
