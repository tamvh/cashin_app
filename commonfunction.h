#ifndef COMMONFUNCTION_H
#define COMMONFUNCTION_H
#include <QString>
#include "datatypes.h"

#define MACHINE_NAME                "VNG"
class Cmnfunc
{
public:
    Cmnfunc();

    static QString  gVPOS1();
    static QString  formatZPUserInfoByPhone(const QString phone_number);
    static QString  formatZPUserInfoByZaloPayID(const QString phone_number);
    static QString  formatTransferByZaloPayID(const QString &zpid, const QString &amount, const QString &transfer_type);
    static QString  formatTransferByPhone(const QString &phone_number, const QString &amount, const QString &transfer_type);
};

#endif // COMMONFUNCTION_H
