#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMultiMap>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void scanPCI();

private:
    QMultiMap<QString, QString> * venDevStr;
    QMultiMap<QString, QString> * venDevNum;
    QPushButton * pScanButton;
    QPushButton * pExitButton;

};

#endif // MAINWINDOW_H
