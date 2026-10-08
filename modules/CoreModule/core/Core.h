#ifndef CORE_H
#define CORE_H

#include <QString>
#include <QRect>

namespace Core {

struct AuthResult
{
    bool success = false;
    QString error;
    int operatorId = -1;
    QString username;
    QString fullName;
};

struct SessionContext
{
    bool active = false;
    QString context;
    int operatorId = -1;
    QString username;
    QString fullName;
    int sessionId = -1;
};

// struct User
// {
//     bool auth = false;
//     QString error;
//     int operatorId = -1;
//     QString username = "Not found";
//     QString fullName = "Not found";
// };

struct Detection
{
    int classId = -1;
    QString className;
    float confidence = 0.0f;
    QRect boundingBox;
};

}

#endif // CORE_H
