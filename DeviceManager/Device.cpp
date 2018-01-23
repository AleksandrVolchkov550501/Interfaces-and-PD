#include "Device.h"

QString Device::getDeviceClass() const
{
    return deviceClass;
}

void Device::setDeviceClass(const QString &value)
{
    deviceClass = value;
}

//QString Device::getDeviceDesc() const
//{
//    return deviceDesc;
//}

//void Device::setDeviceDesc(const QString &value)
//{
//    deviceDesc = value;
//}

QString Device::getGuid() const
{
    return guid;
}

void Device::setGuid(const QString &value)
{
    guid = value;
}

QString Device::getHardwareId() const
{
    return hardwareId;
}

void Device::setHardwareId(const QString &value)
{
    hardwareId = value;
}

QString Device::getManufacturer() const
{
    return manufacturer;
}

void Device::setManufacturer(const QString &value)
{
    manufacturer = value;
}

QString Device::getDriverInfPath() const
{
    return driverInfPath;
}

void Device::setDriverInfPath(const QString &value)
{
    driverInfPath = value;
}

QString Device::getDevicePath() const
{
    return devicePath;
}

void Device::setDevicePath(const QString &value)
{
    devicePath = value;
}

QString Device::getDriverDesc() const
{
    return driverDesc;
}

void Device::setDriverDesc(const QString &value)
{
    driverDesc = value;
}

Device::Device(QObject *parent) : QObject(parent)
{

}
