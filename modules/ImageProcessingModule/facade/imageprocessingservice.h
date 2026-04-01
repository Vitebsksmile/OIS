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


signals:

    //  Сигнал, который должен быть отправлен (emitted) после завершения работы
    //  Сообщает путь к файлу и результат (true — успех, false — провал)
    void imageProcessed(const QUrl &filePath, bool success) override;

    //  Сигнал для передачи конкретного текста ошибки, если что-то пошло не так,
    //  например, «файл не найден» или «недостаточно памяти»
    void processingError(const QUrl &filePath, const QString &error) override;


private:

    //  Хранит путь к файлу, который обрабатывается в данный момент,
    //  чтобы знать, какой путь отправить обратно в сигнале imageProcessed
    QUrl m_currentFilePath;

};

#endif // IMAGEPROCESSINGSERVICE_H
