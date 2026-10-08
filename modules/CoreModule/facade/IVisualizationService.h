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
#include <QImage>
#include "Core.h"

class IDatabaseService;
class ICameraManagerService;
class IImageProcessingService;

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

    virtual bool setDbService(IDatabaseService *dbService) = 0;
    virtual bool setCamService(ICameraManagerService *camService) = 0;
    virtual bool setProcService(IImageProcessingService *procService) = 0;

public slots:
    //  IImageProcessingModule -> this
    virtual void onPreProcessingStartNotification(bool success) = 0;

    //  IImageProcessingModule -> this
    virtual void onImagePreProcessingFinished(const QString &filePath,
                                              bool success) = 0;

    //  IImageProcessingModule -> this
    virtual void onPreProcessingError(const QString &filePath,
                                      const QString &error) = 0;

    //  ????????????? 02.10.26
    //virtual void onFrameReady(const OIS::Core::Frame &frame) = 0;

    //  CameraManagerModule -> this
    virtual void onRawImageFrameReady(const QImage &frame) = 0;

    //  IImageProcessingModule -> this
    virtual void onProcessedFrameReady(const QImage &frame) = 0;

    //  ImageProcessingModule -> this
    virtual void onFrameWithBoxesReady(const QImage &frame,
                                       const std::vector<std::vector<int>> &rectanglePoints) = 0;

    virtual void onMLResult(const Core::Detection &detect) = 0;

    //  DatabaseModule -> this
    virtual void onDefectAdded() = 0;

    //virtual void onDbExecutionError(const QString &error) = 0;

signals:
    //  this -> ImageProcessingModule
    //  Вызываем его через emit, когда в интерфейс приходит команда начать PreProcessing
    void imagePreProcessingRequested(const QString &filePath);
};

//  Factory method
QSharedPointer<IVisualizationService> createVisualizationService(QObject* parent);

#endif // IVISUALIZATIONSERVICE_H
