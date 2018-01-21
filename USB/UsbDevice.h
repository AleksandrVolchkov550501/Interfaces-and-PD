#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <QObject>

class UsbDevice : public QObject
{
    Q_OBJECT

    QString deviceDesc;
    QString friendlyName;
    QString letter;
    QString instanceID;

    quint64 freeSpace;
    quint64 busySpace;
    quint64 totalSpace;

public:
    explicit UsbDevice(QObject *parent = nullptr);
    ~UsbDevice();

    QString getLetter() const;
    void setLetter(const QString &value);

    quint64 getFreeSpace() const;
    void setFreeSpace(const quint64 &value);

    quint64 getBusySpace() const;
    void setBusySpace(const quint64 &value);

    quint64 getTotalSpace() const;
    void setTotalSpace(const quint64 &value);

    QString getDeviceDesc() const;
    void setDeviceDesc(const QString &value);

    QString getFriendlyName() const;
    void setFriendlyName(const QString &value);

    QString getInstanceID() const;
    void setInstanceID(const QString &value);

signals:

public slots:
};

#endif // USBDEVICE_H
