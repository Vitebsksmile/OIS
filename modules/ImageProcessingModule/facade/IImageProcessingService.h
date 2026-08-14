//  Интерфейс модуля (публичное API)

#ifndef IIMAGEPROCESSINGSERVICE_H
#define IIMAGEPROCESSINGSERVICE_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <opencv2/opencv.hpp>

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
    //  VisualizationModule -> this
    virtual void onImagePreProcessingRequested(const QString &filePath) = 0;

    //  ProcessManager -> this
    //  для уведомления User о начале предобработки (for QML about Start)
    virtual void onPreProcessingStartNotification(bool success) = 0;

    //  ProcessManager -> this
    //  для уведомления о завершении предобработки (for QML about Finished)
    virtual void onPreProcessingFinished(const QString &resultFilePath) = 0;

    //  CameraManagerModule -> this
    virtual void onCVFrameReady(const cv::Mat &frame) = 0;

    //  ProcessManager -> this
    virtual void onFrameWithBoxesReady(const QImage &frame
                                       , const std::vector<std::vector<int>> &rectanglePoints) = 0;
signals:
    //  Сигнал для ProcessManager -> создай imagePreProcessing
    void imagePreProcessingRequested(const QString &filePath);  //  Добавить параметры "тип" предобработки

    //  For VisualizationService about Start
    void preProcessingStartNotification(bool success);

    //  Сигнал, который должен быть отправлен (emitted) после завершения работы
    //  Сообщает путь к файлу и результат (true — успех, false — провал)
    //  (for QML about Finished)
    void imagePreProcessingFinished(const QString &filePath, bool success);

    //  Сигнал для передачи конкретного текста ошибки, если что-то пошло не так,
    //  например, «файл не найден» или «недостаточно памяти»
    void prePreProcessingError(const QString &filePath, const QString &error);

    //  this -> ProcessManager
    void processFrame(const cv::Mat &cvFrame);
    //void processedFrameReady(ProcessedFrame frame);

    //  this -> VisualizationModule
    void objectFound(const size_t &objectCount
                     , const std::vector<std::vector<int>> &rectanglePoints);

    void frameWithBoxesReady(const QImage &frame
                             , const std::vector<std::vector<int>> &rectanglePoints);
};

#endif // IIMAGEPROCESSINGSERVICE_H
