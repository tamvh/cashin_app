#include "blemanager.h"

BLEmanager::BLEmanager(QObject *parent) : QObject(parent), localDevice(new QBluetoothLocalDevice)
{
    next_paper = 0;
    pong_rcv = false;
    m_bleInterface = new BLEInterface(this);
    connect(m_bleInterface, &BLEInterface::dataReceived,
                this, &BLEmanager::dataReceived);
    connect(m_bleInterface, &BLEInterface::device_found,
            [this] (QStringList devices, QStringList add ){
        emit clear_list_device();
        for(int i=0; i<devices.length();i++){
            qDebug()<< devices[i];
            emit ble_name(devices[i], add[i]);
        }
    });
    connect(m_bleInterface, &BLEInterface::servicesChanged,
            [this] (QStringList services){
        emit device_connect(true);

    });
    connect(m_bleInterface, &BLEInterface::devivedisconnect,
            [this]{
        emit device_connect(false);
    });
    connect(m_bleInterface,SIGNAL(connectedChanged(bool)),this,SLOT(update_connect_status(bool)));
    connect(m_bleInterface,SIGNAL(get_services_success()),this,SLOT(get_service_ok()));
}

void BLEmanager::disconnectDevice()  {
    m_bleInterface->disconnectDevice();
}

void BLEmanager::connect_to_device(QString _address) {
    qDebug() << "BLEmanager::connect_to_device: " << _address;
    m_bleInterface->connectCurrentDevice(_address);
}

void BLEmanager::update_connect_status(bool _status)
{
    qDebug() << "BLEmanager::update_connect_status(), status: " << _status;
    pong_rcv = _status;
    emit device_connect(_status);
}

void BLEmanager::get_service_ok()
{
    emit get_service_success();
}

void BLEmanager::dataReceived(QByteArray data) {
    qDebug() << "BLEmanager::dataReceived, data: " + QString::fromStdString(data.toStdString());
    pong_rcv = true;
    long long money = QString::fromStdString(data.toStdString()).toLongLong();

    if(money > 0) {
        next_paper += 1;
        QString msg_confirm = "OK";
        QByteArray arr_data;
        arr_data = QByteArray(msg_confirm.toLatin1());
        m_bleInterface->write(arr_data);
        if(next_paper == 1) {
            emit getTotalAmount(QString::fromStdString(data.toStdString()));
        }
    } else  {
        next_paper = 0;
    }
}

bool BLEmanager::isConnected() {
    return m_bleInterface->isConnected();
}

void BLEmanager::power_enable(int _status)
{
    if (_status == 1){
        localDevice->powerOn();
        qDebug() << "power on ble";
    }
    else{
        localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
}

bool BLEmanager::get_pong_rcv() {
    return pong_rcv;
}
