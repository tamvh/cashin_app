#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#define QMessageLogContext QObject
#endif

#pragma warning( disable: 4100 )
#pragma warning( disable: 4200 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4995 )
#pragma warning( disable: 4996 )

using namespace std;

class Logger
{
public:
    enum mode {
        None = 0,
        Fatal = 1,
        Critical = 2,
        Warning = 3,
        Debug = 4
    };

    Logger();
    virtual ~Logger();

    static void debug(const QString& text, const wchar_t *info);
    static void debug(const QString& text);
    static void warning(const QString& text);
    static void critical(const QString& text);
    static void fatal(const QString& text);

    static void setMode(int loglevel);
    static void prodinfo(const char *logfilename, const QString product);
    static void initialize(const char *logfilename, const QString product);

private:
    static void logger_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void autoActive(mode loglevel);

    static bool m_isopen;
    static mode m_mode;
    static ofstream m_logfile;
    static char m_logfname[256];
    static QString m_prodname;
};

#endif /* LOGGER_H_ */
