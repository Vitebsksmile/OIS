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


#include <QObject>

#include "IImageProcessingService.h"


class ProcessManager;

//  Наследуемся от нашего интерфейса
//  Теперь этот класс обязан реализовать метод processImage
class ImageProcessingService : public IImageProcessingService
{

    Q_OBJECT


public:

    explicit ImageProcessingService(QObject *parent = nullptr);


//  Реализация интерфейса IImageProcessingService
public slots:

    //  Слот для получения пути из VisualizationModule
    //  Слово override подтверждает переопределение
    void onImagePreProcessingRequestedFromVisualizationModule(const QString &filePath) override;


    //  Слушает ProcessManager для дальнейшей отправки в VisualizationModule
    //  для уведомления User о начале предобработки (for QML about Start)
    void onPreProcessingStartNotification(bool) override;


signals:

    //  Сигналы объявленные в Интерфейсе в наследнике не объявляются, но используются!!!


private:

    ProcessManager *m_processManager;


    //  Хранит путь к файлу, который обрабатывается в данный момент,
    //  чтобы знать, какой путь отправить обратно в сигнале imageProcessed
    QUrl m_currentFilePath; //  !!!!!???????

};

#endif // IMAGEPROCESSINGSERVICE_H
