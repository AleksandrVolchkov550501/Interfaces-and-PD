#ifndef WIFIINFO_H
#define WIFIINFO_H

#include <QObject>

class wifiInfo : public QObject
{
    Q_OBJECT
public:
    explicit wifiInfo(QObject *parent = nullptr);

signals:

public slots:
};

#endif // WIFIINFO_H