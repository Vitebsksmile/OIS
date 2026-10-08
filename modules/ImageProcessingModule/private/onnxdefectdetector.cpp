#include "onnxdefectdetector.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>

OnnxDefectDetector::OnnxDefectDetector(const QString &modelPath, QObject *parent)
    : m_modelPath(modelPath)
    , QObject(parent)
{
    try {
        m_net = cv::dnn::readNetFromONNX(m_modelPath.toStdString());

        if (m_net.empty()) {
            qWarning()
                << "OnnxDeffectDetector: failed to load ONNX model"
                << m_modelPath;

            return;
        }

        qDebug()
            << "OnnxDefectDetector: ONNX model loaded successfully:"
            << m_modelPath;
    } catch (const cv::Exception &e) {
        qWarning()
            << "OnnxDefectDetector: OpenCV error while loading model:"
            << e.what();
    }
}

bool OnnxDefectDetector::isReady() const
{
    return !m_net.empty();
}

QVector<Core::Detection> OnnxDefectDetector::detect(const cv::Mat &image)
{
    QVector<Core::Detection> detections;

    if (image.empty()) {
        qWarning() << "OnnxDefectDetector: input image is empty.";

        return detections;
    }

    if (m_net.empty()) {
        qWarning() << "OnnxDefectDetector: model is not loaded.";

        return detections;
    }

    //  -----------------------------------------------------------------------
    //  Preprocessing
    //  -----------------------------------------------------------------------
    cv::Mat blob = cv::dnn::blobFromImage(image,
                                          1.0 / 255.0,
                                          cv::Size(640, 640),
                                          cv::Scalar(),
                                          true,
                                          false);

    m_net.setInput(blob);

    //  -----------------------------------------------------------------------
    //  Neural network inference
    //  -----------------------------------------------------------------------
    cv::Mat output;

    try {
        output = m_net.forward();
    } catch (const cv::Exception &e) {
        qWarning()
            << "OnnxDefectDetector: inference error:"
            << e.what();

        return detections;
    }

    //  -----------------------------------------------------------------------
    //  Chek output demensions
    //  -----------------------------------------------------------------------
    if (output.dims != 3) {
        qWarning()
            << "OnnxDefectDetector: unexpected output demensions:"
            << output.dims;

        return detections;
    }

    const int dimensions = output.size[1];
    const int predictions = output.size[2];

    if (dimensions != 10) {
        qWarning()
            << "OnnxDefectDetector: unexpected output size:"
            << dimensions;

        return detections;
    }

    //  -----------------------------------------------------------------------
    //  Convert output to 2D matrix
    //  -----------------------------------------------------------------------
    cv::Mat data(dimensions,
                 predictions,
                 CV_32F,
                 output.ptr<float>());

    //  -----------------------------------------------------------------------
    //  Detection postprocessing
    //  -----------------------------------------------------------------------
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    const float xScale = static_cast<float>(image.cols) / 640.0f;
    const float yScale = static_cast<float>(image.rows) / 640.0f;

    for (int i = 0; i < predictions; i++) {
        const float cx = data.at<float>(0, i);
        const float cy = data.at<float>(1, i);
        const float width = data.at<float>(2, i);
        const float height = data.at<float>(3, i);


        //  -----------------------------------------------------------------------
        //  Find class with maximum probability
        //  -----------------------------------------------------------------------
        float maxScore = 0.0f;
        int classId = -1;

        for (int c = 0; c < 6; c++) {
            const float score = data.at<float>(4 + c, i);

            if (score > maxScore) {
                maxScore = score;
                classId = c;
            }
        }

        // -----------------------------------------------------
        // Confidence filtering
        // -----------------------------------------------------

        if (maxScore < m_confidenceThreshold) {
            continue;
        }

        // -----------------------------------------------------
        // Convert center coordinates to top-left coordinates
        // -----------------------------------------------------
        float x = cx - width / 2.0f;

        float y = cy - height / 2.0f;

        // -----------------------------------------------------
        // Scale coordinates to original image
        // -----------------------------------------------------
        x *= xScale;
        y *= yScale;
        float scaledWidth = width * xScale;

        float scaledHeight = height * yScale;

        cv::Rect box(static_cast<int>(x),
                     static_cast<int>(y),
                     static_cast<int>(scaledWidth),
                     static_cast<int>(scaledHeight));

        // -----------------------------------------------------
        // Clip bounding box to image
        // -----------------------------------------------------
        box &= cv::Rect(0,
                        0,
                        image.cols,
                        image.rows);

        if (box.width <= 0 || box.height <= 0) {
            continue;
        }

        classIds.push_back(classId);
        confidences.push_back(maxScore);
        boxes.push_back(box);
    }

    // ---------------------------------------------------------
    // 6. Non-Maximum Suppression
    // ---------------------------------------------------------

    std::vector<int> indices;

    cv::dnn::NMSBoxes(boxes,
                      confidences,
                      m_confidenceThreshold,
                      m_nmsThreshold,
                      indices);


    // ---------------------------------------------------------
    // 7. Convert OpenCV detections to Core::Detection
    // ---------------------------------------------------------
    for (const int index : indices) {
        const int classId = classIds[index];

        Core::Detection detection;

        detection.classId = classId;

        detection.className = QString::fromStdString(m_classNames[classId]);

        detection.confidence = confidences[index];

        const cv::Rect& box = boxes[index];

        detection.boundingBox = QRect(box.x,
                                      box.y,
                                      box.width,
                                      box.height);

        detections.append(detection);
    }

    return detections;
}


