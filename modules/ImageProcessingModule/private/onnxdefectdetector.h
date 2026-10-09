#ifndef ONNXDEFECTDETECTOR_H
#define ONNXDEFECTDETECTOR_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <QString>
#include <QRect>
#include <QVector>
#include "Core.h"

class OnnxDefectDetector : public QObject
{
public:
    explicit OnnxDefectDetector(const QString &modelPath, QObject *parent = nullptr);
    ~OnnxDefectDetector() = default;

    bool isReady() const;

    QVector<Core::Detection> detect(const cv::Mat &image);

private:
    cv::dnn::Net m_net;
    QString m_modelPath;

    const float m_confidenceThreshold = 0.25f;
    const float m_nmsThreshold = 0.45f;

    const std::vector<std::string> m_classNames = {"open_circuit",
                                                   "short_circuit",
                                                   "mouse_bite",
                                                   "spur",
                                                   "copper_spill",
                                                   "pin_hole"};
    // const QVector<QString> m_classNames = {"open_circuit",
    //                                        "short_circuit",
    //                                        "mouse_bite",
    //                                        "spur",
    //                                        "copper_spill",
    //                                        "pin_hole"};
};

#endif // ONNXDEFECTDETECTOR_H
