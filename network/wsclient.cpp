#include "wsclient.h"
#include <QtCore/QDebug>

#define PING_TIMER 3000

WSClient::WSClient(const QUrl &url, bool debug, QObject *parent) :
    QThread(parent),
    m_url(url),
    m_debug(debug),
    timerPing(0)
{
    if (m_debug) {
        qDebug() << "WebSocket server:" << url;
    }

    connect(&m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WSClient::onClosed);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WSClient::textMessageReceived);
    connect(&m_webSocket, &QWebSocket::pong, this, &WSClient::onPong);
    connect(&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    connect(this, &WSClient::reconnect, this, &WSClient::onReConnect);

    m_webSocket.open(QUrl(url));
}

WSClient::~WSClient()
{
    ////qDebug() << "close websocket client!";
}

void WSClient::start()
{
    timerPing = startTimer(PING_TIMER);
    m_start = true;
    QThread::start();
}

void WSClient::stop()
{
    m_start = false;
}

void WSClient::onPong(quint64 elapsedTime, const QByteArray &payload)
{
    //qDebug() << "Receive pong:" << QString(payload);
    pingFlag = 0;
}
#include <QTimerEvent>
void WSClient::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerPing) {
        //qDebug() << "Timer ping event";
        if (pingFlag > 2) {
            onClosed();
            return;
        }
        if (m_webSocket.isValid()) {
            //qDebug() << "Send ping";
            m_webSocket.ping(QString("ping").toLocal8Bit());
            pingFlag++;
        } else {
            //qDebug() << "Websocket not valid";
        }
    }
}

void WSClient::sendTextMessage(QString message)
{
     qint64 sent = m_webSocket.sendTextMessage(message);
     //qDebug() << "Sent: " << sent;
}

void WSClient::run()
{
    while (m_start)
    {
        QThread::sleep(3);
        if (!m_webSocket.isValid())
        {
            qDebug() << "Reconnect";
            emit this->reconnect();
        }
    }
}

void WSClient::onConnected()
{
    if (m_debug) {
        //qDebug() << "WebSocket connected " << QTime::currentTime().toString("HH:mm:ss.zzz");
    }
    emit connected();
    pingFlag = 0;
}

void WSClient::onTextMessageReceived(QString message)
{
    if (m_debug) {
        //qDebug() << "Message received:" << message;
    }
    emit textMessageReceived(message);
}

void WSClient::onClosed()
{
    if (m_debug) {
        //qWarning() << "WebSocket disconnected " << QTime::currentTime().toString("HH:mm:ss.zzz");
    }
    m_webSocket.close();
    emit closed();
}

void WSClient::onReConnect()
{
    m_webSocket.open(m_url);
}

void WSClient::onError(QAbstractSocket::SocketError error)
{
    //qDebug() << "WSError: " << error;
}
