#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Device : public QObject
{
    Q_OBJECT

    QString deviceClass;
//    QString deviceDesc;
    QString guid;
    QString hardwareId;
    QString manufacturer;
    QString driverInfPath;
    QString driverDesc;
    QString devicePath;

public:
    explicit Device(QObject *parent = nullptr);

    QString getDeviceClass() const;
    void setDeviceClass(const QString &value);

//    QString getDeviceDesc() const;
//    void setDeviceDesc(const QString &value);

    QString getGuid() const;
    void setGuid(const QString &value);

    QString getHardwareId() const;
    void setHardwareId(const QString &value);

    QString getManufacturer() const;
    void setManufacturer(const QString &value);

    QString getDriverInfPath() const;
    void setDriverInfPath(const QString &value);

    QString getDevicePath() const;
    void setDevicePath(const QString &value);

    QString getDriverDesc() const;
    void setDriverDesc(const QString &value);

signals:

public slots:
};

#endif // DEVICE_H
