#ifndef KEEPWAKE_H
#define KEEPWAKE_H

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QtAndroidExtras>
#include <QAndroidJniObject>
#include "jni.h"
#endif

class KeepWake
{
public:
    KeepWake();
    virtual ~KeepWake();

    void    onChange(bool OnOff);

    bool    status;

private:
    void    wakeup();
    void    dim();
    void    keepscreen(bool on);

};

#endif // KEEPWAKE_H
