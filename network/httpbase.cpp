#include <QStringList>
#include "httpbase.h"
#include "httpparams.h"
#include "httptool.h"

#define HTTP_HEADER_CONTENT_TYPE        "Content-Type"
#define HTTP_HEADER_CONTENT_LENGTH      "Content-Length"
#define HTTP_HEADER_USER_AGENT          "User-Agent"
#define HTTP_HEADER_CONTENT_TYPE_FORM   "application/x-www-form-urlencoded"
#define HTTP_HEADER_CONTENT_TYPE_JSON   "application/json"

HttpBase::HttpBase(const QUrl &url, QObject *parent, bool islog) :
    QObject(parent),
    m_url(url),
    m_param(HttpParams()),
    m_username(QString()),
    m_password(QString()),
    m_agentname(QString()),
    m_reply(NULL)
  , m_isLogs(true)
{
    m_isLogs = islog;
}

HttpBase::~HttpBase()
{
    if( m_reply ) {
        delete m_reply;
        //qDebug() << "HttpBase::~HttpBase";
    }
    else {
        //qDebug() << "HttpBase::~HttpBase !!!";
    }
}

void HttpBase::finished()
{
    if (m_reply == NULL)
    {
        return;
    }

    disconnect(m_reply, 0, this, 0);

    if (m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray respone = m_reply->readAll();

        if( m_isLogs ) {
            qDebug() << "respone data:" << QString::fromUtf8(respone);
        }

        emit done(respone);
    }
    else
    {
        const QString errorMessage = m_reply->errorString();

        qCritical() << QString("//error:%1, msg:%2")
                         .arg(m_reply->error())
                         .arg(errorMessage);

        emit error((int)m_reply->error(), errorMessage);
    }

    m_reply->deleteLater();
}

void HttpBase::setUrl(const QUrl &url)
{
    if (url.isValid())
    {
        this->m_url = url;
    }
}

void HttpBase::setAgentname(const QString &agentname)
{
    this->m_agentname = agentname;
}

void HttpBase::setParameter(const HttpParams &params)
{
    this->m_param = params;
}

void HttpBase::addParameter(const QString &key, const QVariant &value, bool reset)
{
    if (reset)
    {
        this->m_param.clear();
    }
    this->m_param.add(key, value);
}

void HttpBase::setUsernamePassword(const QString &username, const QString &password)
{
    this->m_username = username;
    this->m_password = password;
}

QNetworkReply *HttpBase::process(const HTTP_PROCESS processType, QIODevice *iostream)
{
    if( m_isLogs ) {
        qDebug() << "http url:" << m_url.toString() + m_param.toString();
    }

    switch(processType)
    {
    case POST:
        m_reply = this->post();
        break;
    case GET:
        m_reply = this->get();
        break;
    case PUT:
        m_reply = this->put(iostream);
        break;
    default:
        return NULL;
    }

    connect(m_reply, SIGNAL(finished()), this, SLOT(finished()));

    return m_reply;
}

void HttpBase::cancel()
{
    if (this->m_reply)
    {
        this->m_reply->abort();
    }
}

QNetworkReply* HttpBase::get()
{
    QNetworkRequest request = this->createRequest(CONTENT_TYPE_NONE);
    return m_nam.get(request);
}

QNetworkReply* HttpBase::post()
{
    QByteArray postdata = this->m_param.post();
    QNetworkRequest request = this->createRequest(CONTENT_TYPE_FORM, POST);
    return m_nam.post(request, postdata);
}

QNetworkReply* HttpBase::put(QIODevice *iostream)
{
    if (iostream == NULL)
    {
        return NULL;
    }

    QNetworkRequest request = this->createRequest(CONTENT_TYPE_NONE);
    return m_nam.put(request, iostream);
}

QNetworkRequest HttpBase::createRequest(CONTENT_TYPE contentType, const HTTP_PROCESS processType)
{
    QUrl url = this->m_url;

    if (!this->m_username.isEmpty() && !this->m_password.isEmpty())
    {
        url.setUserName(this->m_username);
        url.setPassword(this->m_password);
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    if (processType == GET) {
        QUrlQuery query = this->m_param.get();
        url.setQuery(query);
    }
#else
    if (processType == GET) {
        QUrlQuery query = this->m_param.get();
        url.setQueryItems(query.get());
    }
#endif

#if CSMB_UNIT_TEST
    Logger::debug(QString("\n// url: %1").arg(url.toString()));
#else
    //Logger::debug(QString("//url :%1").arg(url.toString()));
#endif

    QNetworkRequest request(url);

    // Set the user agent.
    if (!this->m_agentname.isEmpty())
    {
        request.setRawHeader(HTTP_HEADER_USER_AGENT, this->m_agentname.toUtf8());
    }

    // Set the content type for each data type
    switch (contentType)
    {
    case CONTENT_TYPE_JSON:
        request.setRawHeader(HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_JSON);
        break;
    case CONTENT_TYPE_FORM:
        request.setRawHeader(HTTP_HEADER_CONTENT_TYPE, HTTP_HEADER_CONTENT_TYPE_FORM);
        break;
    case CONTENT_TYPE_NONE:
    default:
        break;
    }

    return (request);
}
