#include <QtTest>
#include <opencv2/core.hpp>
#include "videocaptureworker.h"

class Tst_VideoCaptureWorker : public QObject
{
    Q_OBJECT

private slots:
    // Тест-кейс 1: Позитивный тест конвертации валидной матрицы OpenCV в QImage
    void test_mat_to_qimage_conversion_success();

    // Тест-кейс 2: Негативный тест (граничное условие) обработки пустой матрицы
    void test_mat_to_qimage_empty_matrix();
};

void Tst_VideoCaptureWorker::test_mat_to_qimage_conversion_success()
{
    // 1. ПОДГОТОВКА ДАННЫХ (Выборка и генерация тестовой матрицы 100x100 пикселей, 3 канала BGR)
    cv::Mat testMat(100, 100, CV_8UC3, cv::Scalar(255, 0, 0)); // Чистый синий цвет

    // Инициализируем тестируемый объект в изоляции (камеру НЕ запускаем!)
    VideoCaptureWorker worker;

    // 2. ВЫПОЛНЕНИЕ ДЕЙСТВИЯ: Вызываем private-метод конвертации благодаря friend-связи
    QImage resultImage = worker.matToQImage(testMat);

    // 3. ВЕРИФИКАЦИЯ РЕЗУЛЬТАТА (Жесткий контроль соответствия формата и геометрии в памяти)
    QVERIFY(!resultImage.isNull());
    QCOMPARE(resultImage.width(), 100);
    QCOMPARE(resultImage.height(), 100);
    QCOMPARE(resultImage.format(), QImage::Format_RGBA8888);
}

void Tst_VideoCaptureWorker::test_mat_to_qimage_empty_matrix()
{
    // Тестируем обработку граничного условия (Пустая матрица пикселей)
    cv::Mat emptyMat;
    VideoCaptureWorker worker;

    // Вызываем метод конвертации для пустой матрицы
    QImage resultImage = worker.matToQImage(emptyMat);

    // Верифицируем, что алгоритм безопасно вернул пустой QImage, а не упал в Segmentation Fault
    QVERIFY(resultImage.isNull());
}

// Фабричная функция для запуска из главного main.cpp
QObject* createTst_VideoCaptureWorker() {
    return new Tst_VideoCaptureWorker();
}

#include "tst_VideoCaptureWorker.moc"
