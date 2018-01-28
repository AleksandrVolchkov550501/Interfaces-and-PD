#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include "wifiInfo.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT


public:
    explicit Form(QWidget *parent = 0);
    ~Form();

    int updateWifiInfo();
    void updateTable();
    QString authAlgorithm(DOT11_AUTH_ALGORITHM authAlg);
    QString cipherAlgorithm(DOT11_CIPHER_ALGORITHM cipherAlg);
    void connectToWifi();
    void startDownload();
    void slotFinished();
    void showPic(QString & strFileName);

private:
    Ui::Form *ui;
    QStringList     tableHeaders;
    QMultiMap<QString, WifiInfo * > wifiPoints;
    QNetworkAccessManager * pQNAM;
    QNetworkReply * pReply;
};

#endif // FORM_H
