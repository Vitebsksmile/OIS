#ifndef ABSTRACTCAMERA_H
#define ABSTRACTCAMERA_H

#include <QObject>

class AbstractCamera : public QObject
{
    Q_OBJECT

public:
    explicit AbstractCamera(QObject *parent);
    virtual ~AbstractCamera() = default;

public slots:

};

#endif // ABSTRACTCAMERA_H
