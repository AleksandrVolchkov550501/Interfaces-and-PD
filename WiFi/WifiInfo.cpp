#include "WifiInfo.h"

QString WifiInfo::getSSID() const
{
    return SSID;
}

void WifiInfo::setSSID(const QString &value)
{
    SSID = value;
}

QString WifiInfo::getSignalQuality() const
{
    return signalQuality;
}

void WifiInfo::setSignalQuality(const QString &value)
{
    signalQuality = value;
}

QString WifiInfo::getMAC() const
{
    return MAC;
}

void WifiInfo::setMAC(const QString &value)
{
    MAC = value;
}

QString WifiInfo::getAuthType() const
{
    return authType;
}

void WifiInfo::setAuthType(const QString &value)
{
    authType = value;
}

QString WifiInfo::getGuid() const
{
    return guid;
}

void WifiInfo::setGuid(const QString &value)
{
    guid = value;
}

WifiInfo::WifiInfo(QObject *parent) : QObject(parent)
{

}
