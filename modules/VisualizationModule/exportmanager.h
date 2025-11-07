#pragma once
#include <QObject>
#include <QImage>
#include <QString>

class ExportManager : public QObject
{
    Q_OBJECT
public:
    explicit ExportManager(QObject *parent = nullptr);
    bool saveImage(const QImage &image, const QString &filePath);
    bool saveReport(const QString &text, const QString &filePath);
};
