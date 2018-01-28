#include "Form.h"
#include "ui_Form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    pQNAM = new QNetworkAccessManager(this);
    tableHeaders << "SSID" << "AuthType" << "MAC" << "Quality";
    ui->pTable->setHorizontalHeaderLabels(tableHeaders);
    updateTable();
    QTimer * timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Form::updateTable);
    connect(ui->connectButton, &QPushButton::pressed, this, &Form::connectToWifi);
    connect(ui->downloadButton, &QPushButton::pressed, this, &Form::startDownload);
    timer->start(20000);

}

Form::~Form()
{
    delete ui;
}

void Form::updateTable()
{
    updateWifiInfo();
    ui->pTable->clearContents();

    int i = 0;
    QTableWidgetItem * pItem;

    foreach (WifiInfo * p, wifiPoints.values()) {
        pItem = new QTableWidgetItem(p->getSSID());
        ui->pTable->setItem(i, 0, pItem);
        pItem = new QTableWidgetItem(p->getAuthType());
        ui->pTable->setItem(i, 1, pItem);
        pItem = new QTableWidgetItem(p->getMAC());
        ui->pTable->setItem(i, 2, pItem);
        pItem = new QTableWidgetItem(p->getSignalQuality());
        ui->pTable->setItem(i, 3, pItem);
        pItem = new QTableWidgetItem(p->getProfName());
        ui->pTable->setItem(i, 4, pItem);
        i++;
    }

}

int Form::updateWifiInfo()
{
    HANDLE hClient = NULL;
    DWORD  dwMaxClient = 2;
    DWORD  dwCurVersion = 0;
    DWORD  dwResult = 0;

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO      pIfInfo = NULL;

    PWLAN_BSS_LIST  pBssList = NULL;
    PWLAN_BSS_ENTRY pBssEntry = NULL;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanOpenHandle failed with error: %u\n", dwResult);
    }

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanEnumInterfaces failed with error: %u\n", dwResult);
    }

    pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[0];


    WlanScan(hClient, &pIfInfo->InterfaceGuid, NULL, NULL, NULL);

    QEventLoop loop;
    QTimer::singleShot(4000, &loop, SLOT(quit()));
    loop.exec();

    dwResult = WlanGetNetworkBssList(hClient, &pIfInfo->InterfaceGuid, NULL, dot11_BSS_type_any, FALSE, NULL, &pBssList);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanGetNetworkBssList failed with error: %u\n", dwResult);
    }
    else {
        wifiPoints.clear();
        for (uint j = 0; j < pBssList->dwNumberOfItems; j++)
        {
            QString mac;
            pBssEntry = (PWLAN_BSS_ENTRY)&pBssList->wlanBssEntries[j];

            for(int i = 0; i < 6; i++)
            {
                mac.append(QString::number(pBssEntry->dot11Bssid[i], 16));
                if(i < 5)
                    mac.append(":");
            }
            WifiInfo * pWifiInfo = new WifiInfo();
            pWifiInfo->setMAC(mac.toUpper());
            pWifiInfo->setSSID(QString((char *)pBssEntry->dot11Ssid.ucSSID));
            pWifiInfo->setSignalQuality(QString("%1 (%2 dBm)").arg(pBssEntry->uLinkQuality).arg(pBssEntry->lRssi));
            wifiPoints.insert(pWifiInfo->getSSID(), pWifiInfo);
        }
    }

    PWLAN_AVAILABLE_NETWORK_LIST pAvNetList = NULL;
    PWLAN_AVAILABLE_NETWORK pAvNet = NULL;

    dwResult = WlanGetAvailableNetworkList(hClient, &pIfInfo->InterfaceGuid, 0, NULL, &pAvNetList);

    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
    }
    else {
        for (uint j = 0; j < pAvNetList->dwNumberOfItems; j++)
        {
            pAvNet = (WLAN_AVAILABLE_NETWORK *)&pAvNetList->Network[j];
            QString ssid((char *)pAvNet->dot11Ssid.ucSSID);
            QList<WifiInfo *> list = wifiPoints.values(ssid);
            foreach (WifiInfo * p, list) {
                p->setAuthType(authAlgorithm(pAvNet->dot11DefaultAuthAlgorithm));
                QString s = QString::fromWCharArray(pAvNet->strProfileName);
                if(p->getProfName().isEmpty())
                    p->setProfName(s);
            }
        }
    }
    if (pBssList != NULL) {
        WlanFreeMemory(pBssList);
    }
    if (pIfList != NULL) {
        WlanFreeMemory(pIfList);
    }
    if (pAvNetList != NULL) {
        WlanFreeMemory(pAvNetList);
    }

    CloseHandle(hClient);

    return 0;

}

QString Form::authAlgorithm(DOT11_AUTH_ALGORITHM authAlg)
{
    switch (authAlg) {
    case DOT11_AUTH_ALGO_80211_OPEN:
        return "802.11 Open";
    case DOT11_AUTH_ALGO_80211_SHARED_KEY:
        return "802.11 Shared";
    case DOT11_AUTH_ALGO_WPA:
        return "WPA";
    case DOT11_AUTH_ALGO_WPA_PSK:
        return "WPA-PSK";
    case DOT11_AUTH_ALGO_WPA_NONE:
        return "WPA-None";
    case DOT11_AUTH_ALGO_RSNA:
        return "RSNA";
    case DOT11_AUTH_ALGO_RSNA_PSK:
        return "RSNA with PSK";
    default:
        return "Other";
    }
}

QString Form::cipherAlgorithm(DOT11_CIPHER_ALGORITHM cipherAlg)
{
    switch (cipherAlg) {
    case DOT11_CIPHER_ALGO_NONE:
        return "None";
    case DOT11_CIPHER_ALGO_WEP40:
        return "WEP-40";
    case DOT11_CIPHER_ALGO_TKIP:
        return "TKIP";
    case DOT11_CIPHER_ALGO_CCMP:
        return "CCMP";
    case DOT11_CIPHER_ALGO_WEP104:
        return "WEP-104";
    case DOT11_CIPHER_ALGO_WEP:
        return "WEP";
    default:
        return "Other";
    }
}

void Form::connectToWifi()
{
    HANDLE hClient = NULL;
    DWORD  dwMaxClient = 2;
    DWORD  dwCurVersion = 0;
    DWORD  dwResult = 0;

    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO      pIfInfo = NULL;

    dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanOpenHandle failed with error: %u\n", dwResult);
    }

    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanEnumInterfaces failed with error: %u\n", dwResult);
    }

    pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[0];

    WifiInfo * pWifiInfo = wifiPoints.values().at(ui->pTable->currentRow());
    qDebug() << "SSID: " << pWifiInfo->getSSID()
             << "ProfName:" << pWifiInfo->getProfName();

    if(pWifiInfo->getProfName().isEmpty())
    {
        QString profileXml = QString( "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>"
                            "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">"
                            "<name>%1</name><SSIDConfig><SSID><name>%1</name></SSID></SSIDConfig>"
                            "<connectionType>ESS</connectionType><connectionMode>auto</connectionMode>"
                            "<autoSwitch>false</autoSwitch><MSM><security><authEncryption>"
                            "<authentication>WPA2PSK</authentication><encryption>AES</encryption>"
                            "<useOneX>false</useOneX></authEncryption><sharedKey>"
                            "<keyType>passPhrase</keyType><protected>false</protected>"
                            "<keyMaterial>%2</keyMaterial></sharedKey></security></MSM></WLANProfile>")
                            .arg(pWifiInfo->getSSID()).arg(ui->keyLineEdit->text());

        WlanSetProfile(hClient, &pIfInfo->InterfaceGuid, WLAN_PROFILE_USER,
                       (LPWSTR)profileXml.utf16(), NULL, FALSE, NULL, &dwResult);
        if (dwResult != ERROR_SUCCESS) {
            qDebug("WlanSetProfile failed with error: %u\n", dwResult);
        }
    }
    else {
            WLAN_CONNECTION_PARAMETERS ConnectP1;

            ConnectP1.wlanConnectionMode = wlan_connection_mode_profile;
            ConnectP1.strProfile = (LPWSTR)pWifiInfo->getProfName().utf16();
            ConnectP1.pDot11Ssid = 0;
            ConnectP1.pDesiredBssidList = 0;
            ConnectP1.dot11BssType = dot11_BSS_type_infrastructure;
            ConnectP1.dwFlags = 0;

            dwResult = WlanConnect(hClient, &pIfInfo->InterfaceGuid, &ConnectP1, NULL);
            if (dwResult != ERROR_SUCCESS) {
                qDebug("WlanConnect failed with error: %u\n", dwResult);
            }
    }
    if (pIfList != NULL) {
        WlanFreeMemory(pIfList);
    }
    CloseHandle(hClient);
}

void Form::startDownload()
{
    QUrl url = ui->urlLine->text();
    qDebug() << url;
    QNetworkRequest request(url);
    pReply = pQNAM->get(request);
    connect(pReply, &QNetworkReply::readyRead, this, &Form::slotFinished);
}

void Form::slotFinished()
{
    QByteArray ba = pReply->readAll();
    pReply->deleteLater();
    QUrl url = ui->urlLine->text();
    QString strFileName = url.path().section('/', -1);
    QFile   file(strFileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(ba);
        file.close();

        if (strFileName.endsWith(".jpg")
            || strFileName.endsWith(".png")
            || strFileName.endsWith(".gif")
           ) {
            showPic(strFileName);
        } else {
            QLabel * plbl = new QLabel(ba);
            plbl->show();
        }
    }
}

void Form::showPic(QString & strFileName)
{
    QPixmap pix(strFileName);
    QLabel* plbl = new QLabel;
    plbl->setPixmap(pix);
    plbl->setFixedSize(pix.size());
    plbl->show();
}
