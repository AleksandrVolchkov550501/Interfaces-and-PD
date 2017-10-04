#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMultiMap>
#include <QTableWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void scanPCI();

private:
    QMultiMap<QString, QString> * venDevStr;
    QMultiMap<QString, QString> * venDevNum;
    QTableWidget * pTable;

};

#endif // MAINWINDOW_H
