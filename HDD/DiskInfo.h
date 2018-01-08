#ifndef DISKINFO_H
#define DISKINFO_H

#include <QObject>
#include <Windows.h>
#include <ntddscsi.h>

class DiskInfo : public QObject
{
    Q_OBJECT
public:
    explicit DiskInfo(QObject *parent = nullptr);
    ~DiskInfo();
    QString getModel() const;
    QString getFirmware() const;
    QString getBusType() const;
    QString getSN() const;

    QString getAtaStdSupportString() const;
    QString getUDMAModsString() const;
    QString getMDMAModsString() const;
    QString getPIOModsString() const;

    quint64 getFreeSpace() const;
    quint64 getBusySpace() const;
    quint64 getTotalSpace() const;

signals:

public slots:

private:
    QStringList busTypeList;

    STORAGE_PROPERTY_QUERY storageProtertyQuery;
    HANDLE diskHandle;

    QString model;
    QString firmware;
    QString busType;
    QString SN;

    quint64 freeSpace;
    quint64 busySpace;
    quint64 totalSpace;

    QString ataStdSupportString;
    QString UDMAModsString;
    QString MDMAModsString;
    QString PIOModsString;

    void updateDiskInfo();
    void updateDiskSpace();
    void updateATA_and_POI_DMA_Support();


};

#endif // DISKINFO_H
