#include "UsbDevice.h"

QString UsbDevice::getDeviceDesc() const
{
    return deviceDesc;
}

void UsbDevice::setDeviceDesc(const QString &value)
{
    deviceDesc = value;
}

QString UsbDevice::getFriendlyName() const
{
    return friendlyName;
}

void UsbDevice::setFriendlyName(const QString &value)
{
    friendlyName = value;
}

QString UsbDevice::getInstanceID() const
{
    return instanceID;
}

void UsbDevice::setInstanceID(const QString &value)
{
    instanceID = value;
}

UsbDevice::UsbDevice(QObject *parent)
{
    setFreeSpace(0);
    setTotalSpace(0);
    setBusySpace(0);
}

UsbDevice::~UsbDevice()
{

}

QString UsbDevice::getLetter() const
{
    return letter;
}

void UsbDevice::setLetter(const QString &value)
{
    letter = value;
}

quint64 UsbDevice::getFreeSpace() const
{
    return freeSpace;
}

void UsbDevice::setFreeSpace(const quint64 &value)
{
    freeSpace = value;
}

quint64 UsbDevice::getBusySpace() const
{
    return busySpace;
}

void UsbDevice::setBusySpace(const quint64 &value)
{
    busySpace = value;
}

quint64 UsbDevice::getTotalSpace() const
{
    return totalSpace;
}

void UsbDevice::setTotalSpace(const quint64 &value)
{
    totalSpace = value;
}
