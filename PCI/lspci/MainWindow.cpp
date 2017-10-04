#include "MainWindow.h"
#include <QSettings>
#include <QDebug>
#include <QMultiMap>
#include <QFile>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    venDevStr = new QMultiMap<QString, QString>();
    venDevNum = new QMultiMap<QString, QString>();

    scanPCI();

    pTable = new QTableWidget(venDevStr->size(), 2);
    QStringList lst;

    lst << "Vendor" << "Device";
    pTable->setHorizontalHeaderLabels(lst);

    QTableWidgetItem * pTableItem;
    int i = 0;

    for(QMultiMap<QString, QString>::iterator iter = venDevStr->begin(); iter != venDevStr->end(); iter++)
    {
        pTableItem = new QTableWidgetItem(iter.key());
        pTable->setItem(i, 0, pTableItem);
        pTableItem = new QTableWidgetItem(iter.value());
        pTable->setItem(i, 1, pTableItem);
        i++;
    }

    QVBoxLayout * pLayout = new QVBoxLayout();
    pTable->resizeColumnsToContents();
    this->resize(pTable->size());
    pLayout->addWidget(pTable);
    this->setLayout(pLayout);
}

Widget::~Widget()
{

}

void Widget::scanPCI()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\PCI",
                           QSettings::NativeFormat);
    QStringList groups = settings.childGroups();
    foreach (QString s, groups) {
        venDevNum->insert(s.mid(4, 4).toLower(), s.mid(13, 4).toLower());
    }

    QFile file("pci.ids.txt");

    file.open(QIODevice::Text | QIODevice::ReadOnly);

    for(QMultiMap<QString, QString>::iterator i = venDevNum->begin(); i != venDevNum->end(); i++)
    {
        file.seek(0);
        QString ven;
        QString dev;

        while(!file.atEnd())
        {
            ven = file.readLine();
            if(ven.left(4) != i.key())
                continue;
            while(!file.atEnd())
            {
                dev = file.readLine();

                if(dev.left(5) != "\t" + i.value())
                    continue;
                venDevStr->insert(ven.mid(6), dev.mid(7));
                break;
            }
            break;
        }
    }
    file.close();
}
