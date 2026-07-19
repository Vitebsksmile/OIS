#ifndef IINTEGRATIONSERVICE_H
#define IINTEGRATIONSERVICE_H

#include <QObject>


class IIntegrationService : public QObject
{

    Q_OBJECT

public:

    explicit IIntegrationService(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~IIntegrationService() = default;

public slots:

    //virtual void runInference(ProcessedFrame frame) = 0;

signals:

    //void resultReady(MLResult result);
};

#endif // IINTEGRATIONSERVICE_H