/*
 * Этот файл описывает конкретную реализацию процессора изображений,
 * которая использует библиотеку OpenCV для расчетов и QtConcurrent для того,
 * чтобы тяжелая обработка не «вешала» интерфейс.
*/

/*
 * Почему это правильный подход:
 * Разделение ответственности: Метод performProcessing ничего не знает о Qt и путях — он просто крутит пиксели.
 * Многопоточность: Благодаря QtConcurrent и QFutureWatcher, тяжелая математика OpenCV уходит в другой поток. Пользователь может продолжать нажимать кнопки в SideBar, пока фото обрабатывается.
 * Безопасность: Использование watcher позволяет избежать проблем с синхронизацией потоков (вам не нужны мьютексы в явном виде).
*/

#ifndef IMAGEPROCESSINGSERVICE_H
#define IMAGEPROCESSINGSERVICE_H


#include <opencv2/opencv.hpp>

//  Класс для управления результатом асинхронных вычислений (которые еще не завершены)
#include <QFuture>

//  Высокоуровневый модуль Qt для запуска задач в отдельных потоках (пуле потоков) одной командой
#include <QtConcurrent/QtConcurrent>


#include "IImageProcessingService.h"


//  Наследуемся от нашего интерфейса
//  Теперь этот класс обязан реализовать метод processImage
class ImageProcessingService : public IImageProcessingService
{

    Q_OBJECT


public:

    explicit ImageProcessingService(QObject *parent = nullptr);


public slots:

    //  Слот для получения пути из VisualizationModule
    //  Реализация метода из интерфейса. «Мост» будет вызывать именно этот метод
    //  Слово override подтверждает переопределение
    void onPreprocessingRequested(const QUrl &filePath) override;


private:

    //  Вспомогательный метод,
    //  который подготовит и запустит задачу в фоновом потоке через QtConcurrent::run
    void processImageAsync(const QUrl &filePath);

    //  Сердце модуля. Здесь будет чистый код OpenCV (фильтры, детекция и т.д.)
    //  Этот метод работает с матрицей cv::Mat и возвращает обработанную матрицу
    cv::Mat performProcessing(const cv::Mat &input);


    //  Специальный «наблюдатель»
    //  Он следит за фоновым потоком и посылает сигнал finished(),
    //  когда OpenCV закончит работу, чтобы мы могли забрать результат
    QFutureWatcher<cv::Mat> *m_watcher = nullptr;

    //  Хранит путь к файлу, который обрабатывается в данный момент,
    //  чтобы знать, какой путь отправить обратно в сигнале imageProcessed
    QUrl m_currentFilePath;

};

#endif // IMAGEPROCESSINGSERVICE_H
