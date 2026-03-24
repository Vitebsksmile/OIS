/*
 * Главные фишки этого кода:
 * Безопасность: Везде стоят проверки на nullptr (if m_fileHandler). Это предотвратит «падение» программы (Segmentation Fault).
 * Слабая связность: FileHandler и IVisualizationService ничего не знают друг о друге напрямую. Весь «клей» находится внутри этого Bridge.
 * Переиспользование: Вы можете заменить FileHandler на другой класс (например, NetworkDownloader), и логика визуализации не изменится.
*/


#include <QDebug>

#include "filehandlerbridge.h"
#include "filehandler.h"


FileHandlerBridge::FileHandlerBridge(FileHandler *FileHandler, QObject *parent)
    : IVisualizationService(parent), m_fileHandler(FileHandler)
//  IVisualizationService(parent): Вызов конструктора базового класса (интерфейса)
//  m_fileHandler(fileHandler): Инициализация внутреннего указателя переданным объектом

{

    //  Проверка на nullptr
    //  Если объект передан, мы сразу «подписываемся» на его события
    if (m_fileHandler)
    {
        //  Подключаемся к сигналу FileHandler о выборе файла
        //  Связываем сигнал selectImage со слотом onSelectImage (в этом мосте)
        connect(m_fileHandler,
                &FileHandler::selectImage,
                this,
                &FileHandlerBridge::onSelectImage);

    }

}


//  Динамическая смена обработчика (setFileHandler)
void FileHandlerBridge::setFileHandler(FileHandler *fileHandler)
{

    //  Если старый обработчик уже был, сначала разрываем связь,
    //  чтобы не получать лишних уведомлений и не вызвать ошибку при удалении объекта
    if (m_fileHandler)
    {
        //  Разрываем подключение к сигналу FileHandler
        disconnect(m_fileHandler,
                   &FileHandler::selectImage,
                   this,
                   &FileHandlerBridge::onSelectImage);

    }

    //  Запоминаем новый объект
    m_fileHandler = fileHandler;

    if (m_fileHandler)
    {
        //  Устанавливаем связь с новым объектом
        connect(m_fileHandler,
                &FileHandler::selectImage,
                this,
                &FileHandlerBridge::onSelectImage);

    }

}


//  Слот onFileSelected (реакция на выбор файла)
void FileHandlerBridge::onSelectImage(const QUrl &path)
{
    qDebug() << "Bridge: File selected in FileHandler: " << path;

    // Здесь можно добавить любую дополнительную логику перед отправкой
    // Например, проверку формата файла, валидацию и т.д.

    // Сигнал для ImageProcessingModule (если нужно)
    //  Передаем путь в ImageProcessingModule через интерфейс
    emit filePathSelected(path);

    // Но по вашей архитектуре, путь должен браться напрямую из FileHandler
    // Поэтому этот метод может быть пустым, если обработка инициируется иначе

}


//  Слот onImageProcessed (успех обработки)
void FileHandlerBridge::onImageProcessed(const QUrl &filePath, bool success)
{
    qDebug() << "Bridge: Image processing result for: " << filePath << " " << success;

    //  Если флаг успеха true, то мы уведомляем пользователя
    if (success)
    {
        //  Вызов сигнала из базового интерфейса
        //  Вероятно, в QML есть компонент, который «слушает» этот сигнал
        //  и показывает красивое всплывающее окошко (Toast/Popup)
        emit showNotification("Success ", "Processing completed: " + filePath.toString());

    }

}


//  Слот onProcessingError (ошибка)
void FileHandlerBridge::onProcessingError(const QUrl &filePath, const QString &error)
{

    qDebug() << "Bridge: Processing error: " << filePath << " " << error;

    //  Если что-то пошло не так (например, файл битый),
    //  то мы пробрасываем текст ошибки прямо в интерфейс
    emit showNotification("Error " , error);

}
