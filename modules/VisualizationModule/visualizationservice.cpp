/*
 * Главные фишки этого кода:
 * Безопасность: Везде стоят проверки на nullptr (if m_fileHandler). Это предотвратит «падение» программы (Segmentation Fault).
 * Слабая связность: FileHandler и IVisualizationService ничего не знают друг о друге напрямую. Весь «клей» находится внутри этого Bridge.
 * Переиспользование: Вы можете заменить FileHandler на другой класс (например, NetworkDownloader), и логика визуализации не изменится.
*/


#include "visualizationservice.h"

#include "filehandlerbridge.h"
#include "filehandler.h"


VisualizationService::VisualizationService(FileHandler *fileHandler, QObject *parent)
    : IVisualizationService(parent), m_fileHandler(fileHandler)
//  IVisualizationService(parent): Вызов конструктора базового класса (интерфейса)
//  m_fileHandler(fileHandler): Инициализация внутреннего указателя переданным объектом

{

    //  Проверка на nullptr
    //  Если объект передан, мы сразу «подписываемся» на его события
    if (m_fileHandler)
    {
        //  Подключаемся к сигналу FileHandler о выборе файла
        //  Связываем сигнал requestPreprocessingImage со слотом onRequestPreprocessingImage
        connect(m_fileHandler, &FileHandler::requestPreprocessing,
                this, &VisualizationService::onPreprocessingRequested);

    }

}


//  Динамическая смена обработчика (setFileHandler)
void VisualizationService::setFileHandler(FileHandler *fileHandler)
{

    //  Если старый обработчик уже был, сначала разрываем связь,
    //  чтобы не получать лишних уведомлений и не вызвать ошибку при удалении объекта
    if (m_fileHandler)
    {
        //  Разрываем подключение к сигналу FileHandler
        disconnect(m_fileHandler, &FileHandler::requestPreprocessing,
                   this, &VisualizationService::onPreprocessingRequested);

    }

    //  Запоминаем новый объект
    m_fileHandler = fileHandler;

    if (m_fileHandler)
    {
        //  Устанавливаем связь с новым объектом
        connect(m_fileHandler, &FileHandler::requestPreprocessing,
                this, &VisualizationService::onPreprocessingRequested);

    }

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


//  Слот onRequestPreprocessingImage (реакция на на отправку файла из FileHandler)
//  Слушает сигнал из FileHandler о старте предобработки
void VisualizationService::onPreprocessingRequested(const QUrl &url)
{
    if (!url.isEmpty())
    {

        qDebug() << "Bridge: The user started preprocessing. Path to image: " << url;

    } else {

        qDebug() << "Bridge: ERROR! Getted empty path";

    }


    // Здесь можно добавить любую дополнительную логику перед отправкой
    // Например, проверку формата файла, валидацию и т.д.

    // Сигнал для ImageProcessingModule (если нужно)
    //  Передаем путь в ImageProcessingModule через интерфейс
    //emit filePathSelected(path);

    // Но по вашей архитектуре, путь должен браться напрямую из FileHandler
    // Поэтому этот метод может быть пустым, если обработка инициируется иначе

}
