/*
 * Главные фишки этого кода:
 * Безопасность: Везде стоят проверки на nullptr (if m_fileHandler). Это предотвратит «падение» программы (Segmentation Fault).
 * Слабая связность: FileHandler и IVisualizationService ничего не знают друг о друге напрямую. Весь «клей» находится внутри этого Bridge.
 * Переиспользование: Вы можете заменить FileHandler на другой класс (например, NetworkDownloader), и логика визуализации не изменится.
*/


#include <QDebug>
#include <QThread>
#include <QtQml/qqml.h>
//#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML

#include "visualizationservice.h"
#include "filehandlermanager.h"


VisualizationService::VisualizationService(QObject *parent)
    : IVisualizationService(parent),
    m_fileHandlerManager(new FileHandlerManager(this, this))
//  IVisualizationService(parent): Вызов конструктора базового класса (интерфейса)
{

    qmlRegisterSingletonInstance(
        "VisualizationModule",  //  URI модуля
        1, 0,                   //  Версия
        "FileHandlerManager",   //  Имя типа в Qml
        m_fileHandlerManager);  //  .data()Указатель на объект

}


//  Слушает сигнал из FileHandler о старте предобработки
void VisualizationService::onImagePreProcessingRequested(const QString &filePath)
{
    if (!filePath.isEmpty())
    {

        qDebug() << "VisualizationService: The user started preprocessing. Path to image: " << filePath;

        //  Отправляем в ImageProcessingModule
        emit imagePreProcessingRequested(filePath);

    } else {

        qDebug() << "VisualizationService: ERROR! Getted empty path";

    }

}


//  From IMageProcessingModule for QML about Start
void VisualizationService::onPreProcessingStartNotification(bool success)
{

    if (success)
    {

        emit preProcessingStartNotification(success);

        qDebug() << "VisualizationService: Предобработка началась...";

    } else {
        qDebug() << "VisualizationService: Предобработка не началась...";
    }

}


//  Слот onImageProcessed (успех обработки)
void VisualizationService::onImagePreProcessingFinished(const QString &filePath, bool success)
{
    qDebug() << "VisualizationService: Image preprocessing result for: " << filePath << " " << success;

    emit imagePreProcessingFinished(filePath);

    //  Если флаг успеха true, то мы уведомляем пользователя
    if (success)
    {
        //  Вызов сигнала из базового интерфейса
        //  Вероятно, в QML есть компонент, который «слушает» этот сигнал
        //  и показывает красивое всплывающее окошко (Toast/Popup)
        //emit showNotification("Success ", "Processing completed: " + filePath.toString());

    }

}


//  Слот onPreProcessingError (ошибка)
void VisualizationService::onPreProcessingError(const QString &filePath, const QString &error)
{

    qDebug() << "VisualizationService: Processing error: " << filePath << " " << error;

    //  Если что-то пошло не так (например, файл битый),
    //  то мы пробрасываем текст ошибки прямо в интерфейс
    //emit showNotification("Error " , error);

}
