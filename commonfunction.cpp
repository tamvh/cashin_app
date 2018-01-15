#include "commonfunction.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDataStream>
#include <QByteArray>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QDebug>

Cmnfunc::Cmnfunc()
{
}

QString Cmnfunc::gVPOS1()
{
    QString s;
    s.reserve(32);

    // 7VShsAFE3S4pS3lijpCkIxCDpzi7ljdS

    s.append('7');
    s.append('V');
    s.append('S');
    s.append('h');
    s.append('s');
    s.append('A');
    s.append('F');
    s.append('E');

    s.append('3');
    s.append('S');
    s.append('4');
    s.append('p');
    s.append('S');
    s.append('3');
    s.append('l');
    s.append('i');

    s.append('j');
    s.append('p');
    s.append('C');
    s.append('k');
    s.append('I');
    s.append('x');
    s.append('C');
    s.append('D');

    s.append('p');
    s.append('z');
    s.append('i');
    s.append('7');
    s.append('l');
    s.append('j');
    s.append('d');
    s.append('S');

    return s;
}

QString Cmnfunc::formatZPUserInfoByPhone(const QString phone_number) {
    QJsonObject jso;
    jso["phone_number"] = phone_number;
    QJsonDocument jsd( jso );
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact).data() );
}

QString Cmnfunc::formatZPUserInfoByZaloPayID(const QString zpid) {
    QJsonObject jso;
    jso["zpid"] = zpid;
    QJsonDocument jsd( jso );
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact).data() );
}

QString  Cmnfunc::formatTransferByZaloPayID(const QString &zpid, const QString &amount, const QString &transfer_type) {
    QJsonObject jso;
    jso["zpid"] = zpid;
    jso["amount"] = amount;
    jso["transfer_type"] = transfer_type;
    jso["machine_name"] = MACHINE_NAME;
    QJsonDocument jsd( jso );
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact).data() );
}

QString  Cmnfunc::formatTransferByPhone(const QString &phone_number, const QString &amount, const QString &transfer_type) {
    QJsonObject jso;
    jso["phone_number"] = phone_number;
    jso["amount"] = amount;
    jso["transfer_type"] = transfer_type;
    jso["machine_name"] = MACHINE_NAME;
    QJsonDocument jsd( jso );
    return QString::fromUtf8( jsd.toJson(QJsonDocument::Compact).data() );
}

