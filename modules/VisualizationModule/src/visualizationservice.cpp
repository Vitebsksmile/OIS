/*
 * Главные фишки этого кода:
 * Безопасность: Везде стоят проверки на nullptr (if m_fileHandler). Это предотвратит «падение» программы (Segmentation Fault).
 * Слабая связность: FileHandler и IVisualizationService ничего не знают друг о друге напрямую. Весь «клей» находится внутри этого Bridge.
 * Переиспользование: Вы можете заменить FileHandler на другой класс (например, NetworkDownloader), и логика визуализации не изменится.
*/


#include <QDebug>

#include "visualizationservice.h"


VisualizationService::VisualizationService(QObject *parent)
    : IVisualizationService(parent)
//  IVisualizationService(parent): Вызов конструктора базового класса (интерфейса)
{

}


//  Слот onImageProcessed (успех обработки)
void VisualizationService::onImageProcessed(const QUrl &filePath, bool success)
{
    qDebug() << "Bridge: Image processing result for: " << filePath << " " << success;

    //  Если флаг успеха true, то мы уведомляем пользователя
    if (success)
    {
        //  Вызов сигнала из базового интерфейса
        //  Вероятно, в QML есть компонент, который «слушает» этот сигнал
        //  и показывает красивое всплывающее окошко (Toast/Popup)
        //emit showNotification("Success ", "Processing completed: " + filePath.toString());

    }

}


//  Слот onProcessingError (ошибка)
void VisualizationService::onProcessingError(const QUrl &filePath, const QString &error)
{

    qDebug() << "Bridge: Processing error: " << filePath << " " << error;

    //  Если что-то пошло не так (например, файл битый),
    //  то мы пробрасываем текст ошибки прямо в интерфейс
    //emit showNotification("Error " , error);

}


//  Слот onRequestPreprocessingImage (реакция на отправку файла из FileHandler)
//  Слушает сигнал из FileHandler о старте предобработки
void VisualizationService::onPreprocessingRequested(const QUrl &url)
{
    if (!url.isEmpty())
    {

        qDebug() << "Bridge: The user started preprocessing. Path to image: " << url;

    } else {

        qDebug() << "Bridge: ERROR! Getted empty path";

    }

}
