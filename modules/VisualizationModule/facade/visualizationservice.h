/*
 *  Сигналы (signals): Только объявляем в .h, никогда не пишем тело в .cpp.
 *  Слоты (public slots): Объявляем в .h и обязательно пишем логику в .cpp.
 *  Методы (Q_INVOKABLE): Объявляем в .h и обязательно пишем логику в .cpp.
*/


#ifndef VISUALIZATIONSERVICE_H
#define VISUALIZATIONSERVICE_H


#include <QObject>

#include "IVisualizationService.h"


class FileHandlerManager;  //  Forward declaration
class FileHandler;  //  Forward declaration

class VisualizationService : public IVisualizationService
{

    Q_OBJECT


public:

    //explicit VisualizationService(FileHandler *fileHandler, QObject *parent = nullptr);
    explicit VisualizationService(QObject *parent = nullptr);


    //  Регистрация FileHandler в фасаде
    void registerFileHandler(FileHandler *fileHandler) override;


//  Реализация интерфейса IVisualizationService
public slots:

    //  Слот для установки/смены FileHandler
    void setFileHandler(FileHandler *fileHandler);


    //  в случае успеха обработки
    void onImageProcessed(const QUrl &url, bool success) override;


    //  в случае ошибки обработки
    void onProcessingError(const QUrl &url, const QString &error) override;


//  Мы не пишем их реализации, Qt сделает это за нас
signals:

    //  Создан для отправки в ImageProcessingModule
    //  Вызываем его через emit, когда в интерфейс приходит команда начать Preprocessing
    void requestPreprocessing(const QUrl &url);


    //  Ищет FileHandler
    void lokingForFileHandler(VisualizationService *self);


//  Слоты для связи внутри модуля
private slots:

    //  Слушает сигнал из FileHandler о старте предобработки
    void onPreprocessingRequested(const QUrl &url);


private:

    //  Указатель на связанный объект FileHandlerManager
    //  Инициализирован nullptr для безопасности
    FileHandlerManager *m_fileHandlerManager = nullptr;


    //  Указатель на связанный объект FileHandler
    //  Инициализирован nullptr для безопасности
    FileHandler *m_fileHandler = nullptr;

};

#endif // VISUALIZATIONSERVICE_H
