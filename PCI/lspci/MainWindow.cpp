#include "MainWindow.h"
#include <QSettings>
#include <QDebug>
#include <QMultiMap>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    venDevStr = new QMultiMap<QString, QString>();
    venDevNum = new QMultiMap<QString, QString>();
    scanPCI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::scanPCI()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Enum\\PCI",
                           QSettings::NativeFormat);
    QStringList groups = settings.childGroups();
    qDebug() << groups << endl;

    foreach (QString s, groups) {
        venDevNum->insert(s.mid(4, 4).toLower(), s.mid(13, 4).toLower());
    }
    qDebug() << *venDevNum << endl;
    QFile file("pci.ids.txt");
    qDebug() << "Файл существует? Ответ: " << file.exists() << endl;

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
                venDevStr->insert(ven, dev);
                break;
            }
            qDebug() << ven << " " << dev << endl;
            break;
        }
    }
    qDebug() << *venDevStr << endl;
    file.close();
}
