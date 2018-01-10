#include "PowerInfo.h"

PowerInfo::PowerInfo()
{
    update();
    startTimer(1000);
}

qint8 PowerInfo::ACLineStatus() const
{
    return sps.ACLineStatus;
}

qint8 PowerInfo::BatteryFlag() const
{
    return sps.BatteryFlag;
}

qint8 PowerInfo::BatteryLifePercent() const
{
    return sps.BatteryLifePercent;
}

qint32 PowerInfo::BatteryLifeTime() const
{
    return sps.BatteryLifeTime;
}

void PowerInfo::timerEvent(QTimerEvent *)
{
    update();
}

void PowerInfo::update()
{
    GetSystemPowerStatus(&sps);

    lifeTime = QTime(sps.BatteryLifeTime / 3600,
                     sps.BatteryLifeTime % 3600 / 60);

    qDebug() << "BYTE  ACLineStatus           = " << sps.ACLineStatus        << endl   <<
                "BYTE  BatteryFlag            = " << sps.BatteryFlag         << endl   <<
                "BYTE  BatteryLifePercent     = " << sps.BatteryLifePercent  << endl   <<
                "DWORD BatteryLifeTime        = " << sps.BatteryLifeTime     << endl;

    emit infoChanged();
}

QTime PowerInfo::getLifeTime() const
{
    return lifeTime;
}
