#ifndef DISKINFO_H
#define DISKINFO_H

#include <QObject>
#include <Windows.h>

class DiskInfo : public QObject
{
    Q_OBJECT
public:
    explicit DiskInfo(QObject *parent = nullptr);
    ~DiskInfo();
signals:

public slots:

private:
    QStringList busTypeList;

    STORAGE_PROPERTY_QUERY storageProtertyQuery;
    HANDLE diskHandle;

    QString productId;
    QString version;
    QString busType;
    QString SN;

};

#endif // DISKINFO_H
