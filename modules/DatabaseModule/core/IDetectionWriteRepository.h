#ifndef IDETECTIONWRITEREPOSITORY_H
#define IDETECTIONWRITEREPOSITORY_H

#include <QObject>

class IDetectionWriteRepository : public QObject
{

};

//  Factory method
QSharedPointer<IDetectionWriteRepository> creatDetectionWriteRepository(QObject *parent);

#endif // IDETECTIONWRITEREPOSITORY_H
