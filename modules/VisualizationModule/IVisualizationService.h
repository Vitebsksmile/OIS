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
#include <QUrl>     //  Класс для работы с URL (удобен для QML, так как пути там — это URL)


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


//  public slots: Методы, которые можно вызывать из других потоков или через connect
//  Слоты для приема пути к обработанному изображению и его результатов (сообщений)
//  из ImageProcessingModule
public slots:

    //  в случае успеха обработки
    virtual void onImageProcessed(const QUrl &filePath, bool success) = 0;//!


    //  в случае ошибки обработки
    virtual void onProcessingError(const QUrl &filePath, const QString &error) = 0;//!

};


#endif // IVISUALIZATIONSERVICE_H
