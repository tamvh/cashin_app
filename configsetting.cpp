#include <QMutex>
#include <QSettings>
#include <QDebug>
#include "configsetting.h"

ConfigSetting* ConfigSetting::m_instance = NULL;

ConfigSetting::ConfigSetting(QObject *parent) : QObject(parent)
{
    Load();
}

ConfigSetting::~ConfigSetting()
{
}

ConfigSetting *ConfigSetting::instance()
{
    static QMutex mutex;

    if (m_instance == NULL)
    {
        mutex.lock();

        if (m_instance == NULL) {
            m_instance = new ConfigSetting();
        }

        mutex.unlock();
    }

    return (m_instance);
}

#include <QStandardPaths>
void ConfigSetting::Load()
{
    QSettings sets("vng", "cashin");
    mac_address        = sets.value(QString("option/mac_address"), QString("")).toString();

}

void ConfigSetting::setMacAddress(const QString &maddress)
{
    if (mac_address != maddress) {
        mac_address = maddress;
        QSettings sets("vng", "cashin");
        sets.setValue(QString("option/mac_address"), mac_address);
        sets.sync();
    }
}
