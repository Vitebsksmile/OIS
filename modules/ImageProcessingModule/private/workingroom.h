#ifndef WORKINGROOM_H
#define WORKINGROOM_H


#include <QObject>
#include <QString>

#include "imagepreprocessing.h"


class WorkingRoom : public QObject
{

    Q_OBJECT


public:

    explicit WorkingRoom(QObject *parent = nullptr);


private:

    //  Метод, к-рый управляет жизненным циклом ImagePreprocessing
    void creatPreprocessingObject(const QString &filePath);


    //  Метод использования методов обработки (по возможности сделать принимающим разное к-во аргументов)
    usePreprocessingObject();

};

#endif // WORKINGROOM_H
