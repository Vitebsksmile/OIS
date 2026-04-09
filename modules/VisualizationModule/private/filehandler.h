/*
 *  Сигналы (signals): Только объявляем в .h, никогда не пишем тело в .cpp.
 *  Слоты (public slots): Объявляем в .h и обязательно пишем логику в .cpp.
 *  Методы (Q_INVOKABLE): Объявляем в .h и обязательно пишем логику в .cpp.
*/


#ifndef FILEHANDLER_H
#define FILEHANDLER_H


#include <QObject>  //  Базовый класс для всех объектов Qt (нужен для сигналов и слотов)
#include <QUrl>     //  Класс для работы с URL (удобен для QML, так как пути там — это URL)
#include <QFile>    //  Класс для работы с файловой системой (чтение, запись, копирование)
#include <QtQml/qqmlregistration.h> //  Макрос для автоматической регистрации класса в системе QML


// Наследуемся от QObject, чтобы использовать мощь Qt (MOC), обеспечивая поддержку рефлексии,
//  механизма сигналов и слотов, а также динамических свойств, не поддерживаемых стандартным C++
class FileHandler : public QObject
{

    //  Обязательный макрос для любого класса, использующего сигналы/слоты (свойства)
    Q_OBJECT


    //  Делает класс доступным в Qml (можно будет написать "FileHandler {}" в .qml)
    QML_ELEMENT


    //  Связываем C++ и Qml: св-во, к-рое Qml будет слушать
    Q_PROPERTY(QUrl currentImagePath READ currentImagePath NOTIFY currentImagePathChanged)
    //  currentImagePath — имя свойства в Qml
    //  READ ... — функция, которая отдает значение
    //  NOTIFY ... — сигнал, который говорит Qml: "значение изменилось, перерисуй интерфейс"
    //  currentImagePathChanged — это «голос» вашего C++ класса, который сообщает графическому
    //  интерфейсу (Qml): «Данные изменились, обнови картинку!»


    Q_PROPERTY(bool directionOut READ directionOut WRITE setDirectionOut NOTIFY directionOutChanged)


public:

    //  Конструктор. explicit запрещает неявное преобразование типов.
    //  parent = nullptr позволяет объекту существовать без родителя.
    explicit FileHandler(QObject *parent = nullptr);


    bool directionOut() const { return m_directionOut; }


    //  setter
    Q_INVOKABLE void setDirectionOut(bool out);


    //  Метод для выбора файла. Вызываем из qml
    Q_INVOKABLE void selectImage(QUrl url);


    //  Метод для сохранения (копирования) файла. Вызываем из qml
    Q_INVOKABLE bool saveImage(QUrl sourceUrl, QUrl targetUrl);


    //  Метод для старта предобработки. Вызываем из qml
    Q_INVOKABLE void startPreprocessing();


    //  метод для подготовки пути
    //  Вспомогательная функция для получения "чистого" пути
    Q_INVOKABLE QString getCleanPath(QUrl url);


    //  Геттер для свойства в Qml currentImagePath
    QUrl currentImagePath() const;


public slots:

    void onImagePreProcessingFinished(const QString &filePath);


//  Мы не пишем их реализации, Qt сделает это за нас
signals:

    void directionOutChanged();


    //  Сигнал: создан для отправки в Qml
    //  Вызываем его через emit, когда m_currentImagePath меняется
    void currentImagePathChanged();


    //  Создан для отправки в VisualizationService
    //  Вызываем его через emit, когда user нажимает кнопку 'SrartPreprocessing'
    void imagePreProcessingRequested(const QString &filePath);


private:

    bool m_directionOut = false;


    //  Внутренняя переменная, где реально хранится путь к файлу
    QUrl m_currentImagePath;


    //  Внутренняя переменная в которой храниться ошибка
    QString m_lastError;

};

#endif // FILEHANDLER_H
