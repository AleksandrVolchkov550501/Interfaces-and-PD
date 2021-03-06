#ifndef POWERINFO_H
#define POWERINFO_H

#include <QObject>
#include <QDebug>
#include <QTimerEvent>
#include <QTime>
#include <windows.h>
#include <powrprof.h>

class PowerInfo : public QObject
{
    Q_OBJECT

public:
    PowerInfo();
    ~PowerInfo();

    qint8  ACLineStatus() const;
    qint8  BatteryFlag() const;
    qint8  BatteryLifePercent() const;
    qint32 BatteryLifeTime() const;

    QTime getLifeTime() const;

protected:
    virtual void timerEvent(QTimerEvent* );

public slots:
    void update();

signals:
    void infoChanged();

private:
    QTime lifeTime;
    _SYSTEM_POWER_STATUS sps;
    GUID * pCurPowerSheme;
    DWORD oldPowerdownTimeout;

};

#endif // POWERINFO_H
