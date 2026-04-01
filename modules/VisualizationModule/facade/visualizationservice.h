/*
 *  Сигналы (signals): Только объявляем в .h, никогда не пишем тело в .cpp.
 *  Слоты (public slots): Объявляем в .h и обязательно пишем логику в .cpp.
 *  Методы (Q_INVOKABLE): Объявляем в .h и обязательно пишем логику в .cpp.
*/


#ifndef VISUALIZATIONSERVICE_H
#define VISUALIZATIONSERVICE_H


#include <QObject>
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include "IVisualizationService.h"


class VisualizationService : public IVisualizationService
{

    Q_OBJECT


public:

    explicit VisualizationService(QObject *parent = nullptr);


//  Реализация интерфейса IVisualizationService
public slots:

    //  в случае успеха обработки
    void onImageProcessed(const QUrl &url, bool success) override;


    //  в случае ошибки обработки
    void onProcessingError(const QUrl &url, const QString &error) override;


//  Мы не пишем их реализации, Qt сделает это за нас
signals:

    //  Создан для отправки в ImageProcessingModule
    //  Вызываем его через emit, когда в интерфейс приходит команда начать Preprocessing
    void requestPreprocessing(const QUrl &url);


//  Слоты для связи внутри модуля
private slots:

    //  Слушает сигнал из FileHandler о старте предобработки
    void onPreprocessingRequested(const QUrl &url);


//private:


};

#endif // VISUALIZATIONSERVICE_H
