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


class IVisualizationService : public QObject
{

    Q_OBJECT


public:

    explicit IVisualizationService(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~IVisualizationService() = default;

//  Перенести в FileHandlerBridge
signals:

    //  Сигнал для отправки пути в ImageProcessingModule
    //  Вызываем его через emit, когда User нажмет кнопку для запуска обработки
    void filePathSelected(const QUrl &filePath);

    void showNotification(const QString &title, const QString &message);


//  Слоты для приема пути к обработанному изображению и его результатов (сообщений)
public slots:

    //  в случае успеха обработки
    virtual void onImageProcessed(const QUrl &filePath, bool success) = 0;//!

    //  в случае ошибки обработки
    virtual void onProcessingError(const QUrl &filePath, const QString &error) = 0;//!

};


#endif // IVISUALIZATIONSERVICE_H
