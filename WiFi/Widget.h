#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QTableWidget>
#include "wifiInfo.h"

class Widget : public QWidget
{
    Q_OBJECT

    QMultiMap<QString, WifiInfo * > wifiPoints;

    QTableWidget *  pTable;
    QStringList     tableHeaders;

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    int update();
    QString authAlgorithm(DOT11_AUTH_ALGORITHM authAlg);
    QString cipherAlgorithm(DOT11_CIPHER_ALGORITHM cipherAlg);
};

#endif // WIDGET_H
