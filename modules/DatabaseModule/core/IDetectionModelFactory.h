#ifndef IDETECTIONMODELFACTORY_H
#define IDETECTIONMODELFACTORY_H

#include <QObject>

class IDetectionModelFactory : public QObject
{

};

//  Factory method
QSharedPointer<IDetectionModelFactory> createDetectionModelFactory(QObject *parent);

#endif // IDETECTIONMODELFACTORY_H
