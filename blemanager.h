#ifndef BLEMANAGER_H
#define BLEMANAGER_H
#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QStringList>
#include <QBluetoothLocalDevice>
#include <QTime>
#include <QDateTime>
#include <QList>
#include "bleinterface.h"

class BLEmanager: public QObject
{
    Q_OBJECT
public:
    explicit BLEmanager(QObject *parent = 0);
    Q_INVOKABLE void disconnectDevice();
    Q_INVOKABLE void connect_to_device(QString _address);
    Q_INVOKABLE void power_enable(int _status);
public:
    bool isConnected();
    bool get_pong_rcv();
signals:
    void ble_name(QString _name, QString _add);
    void device_connect(bool _status);
    void get_service_success();
    void clear_list_device();
    void getTotalAmount(const QString &total_money);
public slots:
    void update_connect_status(bool _status);
    void get_service_ok();
    void dataReceived(QByteArray data);
private:
    BLEInterface *m_bleInterface;
    QBluetoothLocalDevice *localDevice;
    bool m_connected;
    int next_paper;
    bool pong_rcv;
};

#endif // BLEMANAGER_H
