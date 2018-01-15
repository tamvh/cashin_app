#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QThread>

class WSClient : public QThread
{
    Q_OBJECT
public:
    explicit WSClient(const QUrl &url, bool debug = false, QObject *parent = Q_NULLPTR);
    ~WSClient();

    void sendTextMessage(QString message);
    void start();
    void stop();

    void onPong(quint64 elapsedTime, const QByteArray &payload);

    void timerEvent(QTimerEvent *event);
private:
      void run();

signals:
    void connected();
    void textMessageReceived(QString message);
    void closed();
    void reconnect();

public slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();
    void onReConnect();

public slots:
    void onError(QAbstractSocket::SocketError error);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
    bool m_start;

    int timerPing;
    int pingFlag;
};

#endif // WSCLIENT_H
