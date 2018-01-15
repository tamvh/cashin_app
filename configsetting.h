#ifndef CONFIGSETTING_H
#define CONFIGSETTING_H

#include <QObject>

class ConfigSetting : public QObject
{
    Q_OBJECT
public:
    static ConfigSetting *instance();
    explicit ConfigSetting(QObject *parent = 0);
    ~ConfigSetting();

    void Load();
    void Save();

    void setMacAddress(const QString &maddress);

    QString mac_address;
private:
    static ConfigSetting *m_instance;

signals:

public slots:

};

#endif // CONFIGSETTING_H
