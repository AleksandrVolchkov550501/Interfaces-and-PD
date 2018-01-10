#include "PowerInfo.h"

PowerInfo::PowerInfo()
{
    update();
    PowerGetActiveScheme(NULL, &pCurPowerSheme);
    PowerReadDCValueIndex(NULL,
                           pCurPowerSheme,
                           &GUID_VIDEO_SUBGROUP,
                           &GUID_VIDEO_POWERDOWN_TIMEOUT,
                           &oldPowerdownTimeout);
    PowerWriteDCValueIndex(NULL,
                           pCurPowerSheme,
                           &GUID_VIDEO_SUBGROUP,
                           &GUID_VIDEO_POWERDOWN_TIMEOUT,
                           10);
    PowerSetActiveScheme(NULL, pCurPowerSheme);

    startTimer(1000);
}

PowerInfo::~PowerInfo()
{
    PowerWriteDCValueIndex(NULL,
                           pCurPowerSheme,
                           &GUID_VIDEO_SUBGROUP,
                           &GUID_VIDEO_POWERDOWN_TIMEOUT,
                           oldPowerdownTimeout);
    PowerSetActiveScheme(NULL, pCurPowerSheme);
    LocalFree(pCurPowerSheme);
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

    emit infoChanged();
}

QTime PowerInfo::getLifeTime() const
{
    return lifeTime;
}
