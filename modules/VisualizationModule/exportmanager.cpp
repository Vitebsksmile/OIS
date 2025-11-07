#include "exportmanager.h"
#include <QFile>
#include <QTextStream>

ExportManager::ExportManager(QObject *parent)
    : QObject(parent)
{}

bool ExportManager::saveImage(const QImage &image, const QString &filePath)
{
    return image.save(filePath);
}

bool ExportManager::saveReport(const QString &text, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream out(&file);
    out << text;
    file.close();
    return true;
}
