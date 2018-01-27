#ifndef WIFIINFO_H
#define WIFIINFO_H

#include <QObject>

class WifiInfo : public QObject
{
    Q_OBJECT

    QString SSID;
    QString signalQuality;
    QString MAC;
    QString authType;
    QString guid;

public:
    explicit WifiInfo(QObject *parent = nullptr);

    QString getSSID() const;
    void setSSID(const QString &value);

    QString getSignalQuality() const;
    void setSignalQuality(const QString &value);

    QString getMAC() const;
    void setMAC(const QString &value);

    QString getAuthType() const;
    void setAuthType(const QString &value);

    QString getGuid() const;
    void setGuid(const QString &value);

signals:

public slots:
};

#endif // WIFIINFO_H
