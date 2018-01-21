#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QByteArray>
#include "Windows.h"
#include "devguid.h"
#include "SetupAPI.h"
#include "Dbt.h"
#include "devpkey.h"
#include "Regstr.h"
#include <ntddscsi.h>
#include <QSettings>
#include <QMap>
#include <QTableWidget>
#include "Cfgmgr32.h"
#include <QPushButton>

#include "UsbDevice.h"

class Widget : public QWidget
{
    Q_OBJECT

    QTableWidget *  pTable;
    QStringList     tableHeaders;
    QPushButton *   ejectButton;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:

    QList<UsbDevice *> usbDevices;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    virtual void timerEvent(QTimerEvent* );

public slots:
    void updateUsbDevices();
    void updateTable();
    QMap<QString, QString> getInstanceIdToLetterMap();
    void ejectDevice();
};

#endif // WIDGET_H
