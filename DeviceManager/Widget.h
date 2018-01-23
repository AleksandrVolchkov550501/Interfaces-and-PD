#ifndef WIDGET_H
#define WIDGET_H

#include <Windows.h>
#include <devpkey.h>
#include <QWidget>
#include <QDebug>
#include <QByteArray>

#include <devguid.h>
#include <SetupAPI.h>

//#include "Dbt.h"
#include <QMap>
#include <QTableWidget>
#include "Cfgmgr32.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QPushButton>
#include "Device.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <QMultiMap>
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT

    QStandardItemModel * model;
    QTreeView          treeView;
    QMultiMap<QString, Device *> multiMap;

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void updateModel();
    void updateDevInfo();
};

#endif // WIDGET_H
