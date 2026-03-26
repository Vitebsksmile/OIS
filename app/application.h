/*
 * Этот заголовочный файл описывает «сердце» программы — класс Application.
 * Он выполняет роль Оркестратора (или Root-контейнера), который создает все ключевые модули,
 * владеет ими и связывает их друг с другом.
*/


#ifndef APPLICATION_H
#define APPLICATION_H


#include <QObject>          //  Базовый класс для работы с сигналами и слотами
#include <QSharedPointer>   //  Подключение «умных» указателей Qt. Они автоматически удалят объекты, когда те станут не нужны, предотвращая утечки памяти


//  Forward Declaration
//  Ускоряет компиляцию и убирает лишние зависимости в хедере.
//  Компилятору достаточно знать, что это классы.
class IVisualizationService;
class IImageProcessingService;


//  Главный связующий класс
class Application : public QObject
{

    Q_OBJECT


public:

    explicit Application(QObject *parent = nullptr);
    ~Application();

    //  Метод для инициализации приложения (создание объектов, настройка связей).
    //  Это лучше делать в отдельном методе, а не в конструкторе.
    void initialize();


private:

    //  Вспомогательный метод, где будут собраны все connect(...) между модулями.
    void setupConnections();

    //  Внутренний обработчик: когда файл выбран, Application решит, кому его передать дальше.
    void onFileSelectedFromHandler(const QUrl &url);

    //  Умный указатель на интерфейс VisualizationModule
    QSharedPointer<IVisualizationService> m_visualizationService;

    //  Умный указатель на интерфейс ImageProcessingModule.
    QSharedPointer<IImageProcessingService> m_imageProcessingService;

};

#endif // APPLICATION_H
