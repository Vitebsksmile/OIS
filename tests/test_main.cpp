#include <QtTest>
#include <QGuiApplication>

// Объявляем внешние тестовые классы (компилятор найдет их в соседних .cpp)
extern QObject* createCameraManagerModuleTest();
extern QObject* createFunctionalAutomationTest();
extern QObject* createTst_VideoCaptureWorker();

int main(int argc, char *argv[])
{
    // Инициализируем графическое приложение Qt.
    // Это автоматически создаст Event Loop и исправит ошибки QWARN/QFATAL!
    QGuiApplication app(argc, argv);

    int status = 0;

    // 1. Запуск изолированных модульных тестов (Unit Testing)
    // {
    //     QScopedPointer<QObject> unitTest(createCameraManagerModuleTest());
    //     status |= QTest::qExec(unitTest.data(), argc, argv);
    // }

    {
        QScopedPointer<QObject> unitTest(createTst_VideoCaptureWorker());
        status |= QTest::qExec(unitTest.data(), argc, argv);
    }

    // 2. Запуск сквозных функциональных тестов (Functional Testing)
    // {
    //     QScopedPointer<QObject> functionalTest(createFunctionalAutomationTest());
    //     status |= QTest::qExec(functionalTest.data(), argc, argv);
    // }

    return status;
}