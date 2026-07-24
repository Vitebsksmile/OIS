#ifndef ICAMERADRIVER_H
#define ICAMERADRIVER_H

#include <string>


class ICameraDriver
{
public:
    virtual ~ICameraDriver() = default;

    virtual bool connect() = 0;

    virtual void disconnect() = 0;

    virtual bool isConncted() const = 0;

    virtual void setParameter(
        const std::string& name,
        double value
    ) = 0;

    virtual double getParameter(
        const std::string& name
    ) const = 0;

    //virtual Parameter parameter() = 0;

};

#endif // ICAMERADRIVER_H
