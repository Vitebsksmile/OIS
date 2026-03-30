//  Интерфейс модуля (публичное API)


#ifndef IIMAGEPROCESSINGSERVICE_H
#define IIMAGEPROCESSINGSERVICE_H


#include <QObject>
#include <QString>
#include <QUrl>


//  Префикс I в названии — общепринятое обозначение интерфейса (Interface)
class IImageProcessingService : public QObject
{

    Q_OBJECT


public:

    //  explicit — запрещает неявное приведение типов
    explicit IImageProcessingService(QObject *parent = nullptr) : QObject(parent) {}


    //  Виртуальный деструктор
    //  Критически важен для интерфейсов: он гарантирует,
    //  что при удалении объекта через указатель на интерфейс
    //  будет вызван деструктор именно дочернего (реального) класса
    virtual ~IImageProcessingService() = default;


//  Логика обработки (слоты)
//  public slots: Методы, которые можно вызывать из других потоков или через connect
public slots:

    //  "= 0" в объявлении виртуального метода означает,
    //  что метод является чисто виртуальным (pure virtual)
    //  Это делает класс абстрактным: нельзя создать объект IImageProcessingService,
    //  обязательно нужно создать класс-наследник и реализовать этот метод там

    //  Слот для получения пути из VisualizationModule
    //  Метод для обработки изображения по пути
    //  Принимает путь к файлу для обработки
    virtual void onPreprocessingRequested(const QUrl &filePath) = 0;


//  Секция событий, на которые могут подписываться другие части программы
//  Сигналы для отправки результатов в VisualizationModule
signals:

    //  Сигнал, который должен быть отправлен (emitted) после завершения работы
    //  Сообщает путь к файлу и результат (true — успех, false — провал)
    void imageProcessed(const QUrl &filePath, bool success);

    //  Сигнал для передачи конкретного текста ошибки, если что-то пошло не так,
    //  например, «файл не найден» или «недостаточно памяти»
    void processingError(const QUrl &filePath, const QString &error);

};

#endif // IIMAGEPROCESSINGSERVICE_H
