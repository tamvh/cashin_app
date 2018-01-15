#ifndef HTTPBASE_H
#define HTTPBASE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariant>
#include <QList>
#include <QPair>
#include "httpparams.h"

typedef enum
{
    HTTP,
    HTTPS
} HTTP_TYPE;

typedef enum
{
    GET,
    POST,
    PUT
} HTTP_PROCESS;

typedef enum
{
    CONTENT_TYPE_NONE = 0,
    CONTENT_TYPE_FORM = 1,
    CONTENT_TYPE_JSON = 2
} CONTENT_TYPE;

class HttpBase : public QObject
{
    Q_OBJECT
protected:
    QNetworkAccessManager   m_nam;
    QUrl                    m_url;
    HttpParams              m_param;
    QString                 m_agentname;
    QString                 m_username;
    QString                 m_password;

private:
    QNetworkReply           *m_reply;
    bool                    m_isLogs;

public:
    HttpBase(const QUrl &url=QString(""), QObject *parent = NULL, bool islog = false);
    ~HttpBase();

    void            setUrl(const QUrl &url);
    void            setAgentname(const QString &agentname);
    void            setParameter(const HttpParams &params);
    void            addParameter(const QString &key, const QVariant &value, bool reset = false);
    void            setUsernamePassword(const QString &username, const QString &password);

    void            cancel();

protected:
    QNetworkReply*  get();
    QNetworkReply*  post();
    QNetworkReply*  put(QIODevice *iostream);
    QNetworkRequest createRequest(CONTENT_TYPE contentType = CONTENT_TYPE_NONE, const HTTP_PROCESS processType = GET);

signals:
    void    done(const QVariant& outputObject);
    void    error(const int &errorCode, const QString &errorMessage);

public slots:
    QNetworkReply* process(const HTTP_PROCESS processType = GET, QIODevice *iostream = NULL);
    void    finished();

};

#endif // HTTPBASE_H
