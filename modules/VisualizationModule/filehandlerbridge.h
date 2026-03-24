#ifndef FILEHANDLERBRIDGE_H
#define FILEHANDLERBRIDGE_H

#include <QObject>

#include "IVisualizationService.h"


class FileHandler;  //  Forward declaration


class FileHandlerBridge : public IVisualizationService
{
    Q_OBJECT

public:

    explicit FileHandlerBridge(FileHandler *fileHandler, QObject *parent = nullptr);

    //  Методы для установки/смены FileHandler
    void setFileHandler(FileHandler *fileHandler);


//  Реализация интерфейса IVisualizationService
public slots:

    void onImageProcessed(const QUrl &filePath, bool success) override;
    void onProcessingError(const QUrl &filePath, const QString &error) override;


private slots:

    //  Слот для сигнала из FileHandler
    //  Слушает сигнал из FileHandler о выборе файла
    void onSelectImage(const QUrl &path);


private:

    //  Указатель на связанный объект FileHandler
    //  Инициализирован nullptr для безопасности
    FileHandler *m_fileHandler = nullptr;

};

#endif // FILEHANDLERBRIDGE_H
