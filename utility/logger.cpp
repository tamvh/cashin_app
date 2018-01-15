#include <QtCore/QDebug>
#include <QTime>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <qlogging.h>
#endif
#include "logger.h"

#define LOG_PREFIX(logtype) QTime::currentTime().toString("hh:mm:ss.zzz").toStdString().data() << " [" << (int)logtype << "] "//
//#define LOG_PREFIX(logtype) ""

bool Logger::m_isopen = false;
Logger::mode Logger::m_mode = Logger::Debug;
ofstream Logger::m_logfile;
char Logger::m_logfname[256] = "\0";
QString Logger::m_prodname = "";

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::debug(const QString& text, const wchar_t *info)
{
//    if (m_mode >= Debug) {
//        QString log = text;
//        if (info)
//        {
//            for (int i=0; i<(int)wcslen(info); i++){
//                log += info[i];
//            }
//        }

//        qDebug(log.toStdString().c_str());
//    }
}

void Logger::debug(const QString& text) {
    if (m_mode >= Debug) {
        qDebug() << text;
    }
}

void Logger::warning(const QString& text)
{
    autoActive(Warning);

    if (m_mode >= Warning) {
        qWarning(text.toStdString().c_str());
    }
}

void Logger::critical(const QString& text)
{
    autoActive(Critical);

    if (m_mode >= Critical) {
        qCritical(text.toStdString().c_str());
    }
}

void Logger::fatal(const QString& text)
{
    autoActive(Fatal);

    if (m_mode >= Fatal) {
        qFatal(text.toStdString().c_str());
    }
}

void Logger::setMode(int loglevel)
{
    m_mode = (mode)loglevel;
}

void Logger::logger_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type)
    {
    case QtDebugMsg:
        m_logfile << LOG_PREFIX(QtDebugMsg) << msg.toStdString().data() << "\n";
        break;
    case QtCriticalMsg:
        m_logfile << LOG_PREFIX(QtCriticalMsg) << msg.toStdString().data() << "\n";
        break;
    case QtWarningMsg:
        m_logfile << LOG_PREFIX(QtWarningMsg) << msg.toStdString().data() << "\n";
        break;
    case QtFatalMsg:
        m_logfile << LOG_PREFIX(QtFatalMsg) << msg.toStdString().data() << "\n";
        abort();
    }

    m_logfile.flush();
}

void Logger::autoActive(mode)
{
//    if ( (strcmp(m_logfname, "")!=0) && (loglevel>m_mode) && !m_prodname.isEmpty() )
//    {
//        m_mode = (mode)loglevel;
//        initialize(m_logfname, m_prodname);
//    }
}

void Logger::prodinfo(const char *logfilename, const QString product)
{
    strcpy(m_logfname, logfilename);
    m_prodname = product;
}

void Logger::initialize(const char *logfilename, const QString product)
{
    if (!m_isopen)
    {
        m_isopen = true;

        m_logfile.open(logfilename, ios::app);

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        qInstallMessageHandler(logger_handler);
#endif

        critical(QString::fromUtf8("%1 - Build date: %2").arg(product).arg(__DATE__));
    }
}
