#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <QString>
//#include "dbrecord.h"

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

}

//Q_DECLARE_METATYPE(Core::AuthContext)

#endif // AUTHENTICATE_H
