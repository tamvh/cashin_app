#include "keepwake.h"
#include <QObject>

KeepWake::KeepWake():
    status(false)
{
}

void KeepWake::onChange(bool OnOff)
{
    if( status != OnOff )
    {
        status = OnOff;

        if( status )
        {
            wakeup();
        }

        keepscreen(status);
    }
}

void KeepWake::wakeup()
{
#ifdef Q_OS_ANDROID

    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if ( activity.isValid() )
    {
        QAndroidJniObject serviceName = QAndroidJniObject::getStaticObjectField<jstring>("android/content/Context","POWER_SERVICE");
        if ( serviceName.isValid() )
        {
            QAndroidJniObject powerMgr = activity.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", serviceName.object<jobject>());
            if ( powerMgr.isValid() )
            {
                jint levelAndFlags1 = QAndroidJniObject::getStaticField<jint>("android/os/PowerManager","SCREEN_DIM_WAKE_LOCK");
                jint levelAndFlags2 = QAndroidJniObject::getStaticField<jint>("android/os/PowerManager","ACQUIRE_CAUSES_WAKEUP");
                jint levelAndFlags = levelAndFlags1 | levelAndFlags2;

                QAndroidJniObject tag = QAndroidJniObject::fromString("gbc smartlight");

                QAndroidJniObject wakeLock = powerMgr.callObjectMethod("newWakeLock", "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;", levelAndFlags, tag.object<jstring>());
                if ( wakeLock.isValid() )
                {
                    wakeLock.callMethod<void>("acquire", "()V");
                    wakeLock.callMethod<void>("release", "()V");
                }
            }
        }
    }

#endif   
}

void KeepWake::dim()
{
#ifdef Q_OS_ANDROID

    qDebug() << "Android OS ...";

    bool on;
    //QtAndroid::runOnAndroidThread([on] {

        qDebug() << "dim runOnAndroidThread";

        QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
        if ( activity.isValid() )
        {
            qDebug() << "activity ...";

            QAndroidJniObject serviceName = QAndroidJniObject::getStaticObjectField<jstring>("android/content/Context","POWER_SERVICE");
            if ( serviceName.isValid() )
            {
                qDebug() << "service ... ";

                QAndroidJniObject powerMgr = activity.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;", serviceName.object<jobject>());
                if ( powerMgr.isValid() )
                {
                    jint levelAndFlags1 = QAndroidJniObject::getStaticField<jint>("android/os/PowerManager","SCREEN_DIM_WAKE_LOCK");
                    jint levelAndFlags2 = QAndroidJniObject::getStaticField<jint>("android/os/PowerManager","ACQUIRE_CAUSES_WAKEUP");

                    qDebug() << "SCREEN_DIM_WAKE_LOCK value:" << levelAndFlags1;
                    qDebug() << "ACQUIRE_CAUSES_WAKEUP value:" << levelAndFlags2;

                    jint levelAndFlags = levelAndFlags1 | levelAndFlags2;
                    qDebug() << "flag1 | flag2 value:" << levelAndFlags;

                    QAndroidJniObject tag = QAndroidJniObject::fromString("gbc smartlight");

                    QAndroidJniObject wakeLock = powerMgr.callObjectMethod("newWakeLock", "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;", levelAndFlags, tag.object<jstring>());
                    if ( wakeLock.isValid() )
                    {
                        qDebug() << " ... ok !";
                        wakeLock.callMethod<void>("acquire", "()V");
                        wakeLock.callMethod<void>("release", "()V");
                    }
                }
            }
        }
    //});

#endif
}

void KeepWake::keepscreen(bool on)
{
#ifdef Q_OS_ANDROID

    QtAndroid::runOnAndroidThread([on] {
        QAndroidJniObject activity = QtAndroid::androidActivity();
        if (activity.isValid())
        {
            QAndroidJniObject window =
                    activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

            if (window.isValid())
            {
                static const int FLAG_KEEP_SCREEN_ON = QAndroidJniObject::getStaticField<jint>("android/view/WindowManager$LayoutParams", "FLAG_KEEP_SCREEN_ON");

                if (on) {
                    window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                } else {
                    window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                }
            }
        }

        QAndroidJniEnvironment env;
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
    });

#endif
}

KeepWake::~KeepWake()
{
}
