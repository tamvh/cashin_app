#ifndef DATATYPES_H
#define DATATYPES_H
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

typedef struct tagArea
{
    long area_id;
    QString area_name;
    tagArea()
    {
        area_id = 0;
        area_name = "";
    }

    tagArea(long id, QString name)
    {
        area_id = id;
        area_name = name;
    }
} AREA, *LAREA;

typedef struct tagLight
{
    int     area_id;
    long    light_id;
    QString light_code;
    QString light_name;
    int     light_type;
    int     status;
    int     onoff;
    int     brightness;
    tagLight()
    {
        area_id     = 0;
        light_id    = 0;
        light_code  = "";
        light_name  = "";
        light_type  = 0;
        status      = 0;
        onoff       = 0;
        brightness  = 0;
    }

    tagLight(long id, QString code, QString name, int offon, int type, int sts, int brghtns, int area)
    {
        area_id     = area;
        light_id    = id;
        light_code  = code;
        light_name  = name;
        light_type  = type;
        status      = sts;
        onoff       = offon;
        brightness  = brghtns;
    }

    void parseArea(const QString &data)
    {
        int err = -1;

        QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray(data.toUtf8()));
        QJsonObject jso = jdoc.object();

        if( jso.contains("err") ) {
            err = jso["err"].toInt();
        }

        if( (err == 0) && jso.contains("dt") )
        {
            QJsonObject jdt = jso["dt"].toObject();

            if( jdt.contains("area") )
            {
                QJsonObject jarea = jdt["area"].toObject();

                area_id     = jarea["area_id"].toInt();
                status      = jarea["status"].toInt();
                onoff       = jarea["area_on_off"].toInt();
                brightness  = jarea["area_brightness"].toInt();
            }
        }
    }

} LIGHT, *LLIGHT;

#endif // DATATYPES_H
