#include "Widget.h"
#include <QPushButton>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    update();
    tableHeaders << "SSID" << "AuthType" << "MAC" << "Quality";
    pTable = new QTableWidget(wifiPoints.size(), tableHeaders.size());
    int i = 0;
    QTableWidgetItem * pItem;

    foreach (WifiInfo * p, wifiPoints.values()) {
        pItem = new QTableWidgetItem(p->getSSID());
        pTable->setItem(i, 0, pItem);
        pItem = new QTableWidgetItem(p->getAuthType());
        pTable->setItem(i, 1, pItem);
        pItem = new QTableWidgetItem(p->getMAC());
        pTable->setItem(i, 2, pItem);
        pItem = new QTableWidgetItem(p->getSignalQuality());
        pTable->setItem(i, 3, pItem);
        i++;
    }

    QPushButton * pButton = new QPushButton("Сканировать!");
    QVBoxLayout * pLayout = new QVBoxLayout();
    pLayout->addWidget(pTable);
    pLayout->addWidget(pButton);
    this->setLayout(pLayout);
    connect(pButton, &QPushButton::pressed, this, &Widget::update);

}

Widget::~Widget()
{

}

int Widget::update()
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

    dwResult = WlanGetNetworkBssList(hClient, &pIfInfo->InterfaceGuid, NULL, dot11_BSS_type_any, FALSE, NULL, &pBssList);
    if (dwResult != ERROR_SUCCESS) {
        qDebug("WlanGetNetworkBssList failed with error: %u\n", dwResult);
    }
    else {
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

//            qDebug () << "MAC: " << mac.toUpper();
//            qDebug () << "SSID: " << pWifiInfo->getSSID();
//            qDebug () << "Signal Quality: " << pWifiInfo->getSignalQuality();

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

QString Widget::authAlgorithm(DOT11_AUTH_ALGORITHM authAlg)
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

QString Widget::cipherAlgorithm(DOT11_CIPHER_ALGORITHM cipherAlg)
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


