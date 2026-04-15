//  Интерфейс модуля (публичное API)

/*  Абстрактный — содержит только чисто виртуальные методы (pure virtual).
*   Задаёт контракт — определяет, что должен уметь делать класс, но не содержит реализации.
*   Предназначен для наследования — от него будут наследоваться конкретные реализации
*
*   Абстракция — клиентский код работает с IVisualizationService*, не привязываясь к конкретной реализации.
*   Тестируемость — легко подменить реальный класс моком (mock) при юнит-тестировании.
*   Разделение ответственности — интерфейсы можно выносить в отдельные модули/public-заголовки, скрывая реализации.
*   Полиморфизм — позволяет использовать разные реализации через единый интерфейс.
*/


#ifndef IVISUALIZATIONSERVICE_H
#define IVISUALIZATIONSERVICE_H


#include <QObject>
#include <QString>


//class FileHandler;  //  Forward declaration

//  Префикс I в названии — общепринятое обозначение интерфейса (Interface)
class IVisualizationService : public QObject
{

    Q_OBJECT


public:

    //  explicit — запрещает неявное приведение типов
    explicit IVisualizationService(QObject *parent = nullptr) : QObject(parent) {}


    //  Виртуальный деструктор
    //  Критически важен для интерфейсов: он гарантирует,
    //  что при удалении объекта через указатель на интерфейс
    //  будет вызван деструктор именно дочернего (реального) класса
    virtual ~IVisualizationService() = default;


    //  Метод установки данных для отправки в другой модуль
    bool setData(const QString &filePath);


//  public slots: Методы, которые можно вызывать из других потоков или через connect
//  Слоты для приема пути к обработанному изображению и его результатов (сообщений)
//  из ImageProcessingModule
public slots:


    //  Слушает сигнал из FileHandler о старте предобработки
    virtual void onImagePreProcessingRequested(const QString &filePath) = 0;


    //  From IImageProcessingModule for QML about Start
    virtual void onPreProcessingStartNotification(bool success) = 0;


    //  в случае успеха предварительной обработки
    virtual void onImagePreProcessingFinished(const QString &filePath, bool success) = 0;


    //  в случае ошибки обработки
    virtual void onPreProcessingError(const QString &filePath, const QString &error) = 0;


signals:

    //  Создан для отправки в ImageProcessingModule
    //  Вызываем его через emit, когда в интерфейс приходит команда начать PreProcessing
    void imagePreProcessingRequested(const QString &filePath);


    //  To FileHandler about started PreProcessing
    void preProcessingStartNotification(bool success);


    //  To FileHandler about finished
    void imagePreProcessingFinished(const QString &filePath);

};


#endif // IVISUALIZATIONSERVICE_H
