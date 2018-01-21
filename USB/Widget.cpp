#include "Widget.h"
#include <QStyle>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    updateUsbDevices();
    tableHeaders << "Устройство" << "Имя" << "Буква" << "Занято" << "Свободно" << "Всего";
    pTable = new QTableWidget(15, tableHeaders.size());
    pTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateTable();
    this->resize(800, 400);
    QVBoxLayout * pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pTable);
    ejectButton = new QPushButton("Извлечь");
    pLayout->addWidget(ejectButton);
    connect(ejectButton, &QPushButton::pressed, this, &Widget::ejectDevice);
    this->setLayout(pLayout);
//    startTimer(2000);
}

Widget::~Widget()
{

}

void Widget::updateTable()
{
    pTable->clear();
    pTable->setHorizontalHeaderLabels(tableHeaders);
    updateUsbDevices();

    QTableWidgetItem * pTableItem;

    int i = 0;

    foreach (UsbDevice * device, usbDevices) {
        if(device->getLetter().isEmpty())
            pTableItem = new QTableWidgetItem(device->getDeviceDesc());
        else
            pTableItem = new QTableWidgetItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)), device->getDeviceDesc());
        pTable->setItem(i, 0, pTableItem);
        pTableItem = new QTableWidgetItem(device->getFriendlyName());
        pTable->setItem(i, 1, pTableItem);
        pTableItem = new QTableWidgetItem(device->getLetter());
        pTable->setItem(i, 2, pTableItem);
        pTableItem = new QTableWidgetItem(device->getBusySpace() ? QString::number(device->getBusySpace()/(1024*1024)) + " МВ" : "-");
        pTable->setItem(i, 3, pTableItem);
        pTableItem = new QTableWidgetItem(device->getFreeSpace() ? QString::number(device->getFreeSpace()/(1024*1024)) + " МВ" : "-");
        pTable->setItem(i, 4, pTableItem);
        pTableItem = new QTableWidgetItem(device->getTotalSpace() ? QString::number(device->getTotalSpace()/(1024*1024)) + " МВ" : "-");
        pTable->setItem(i, 5, pTableItem);
        i++;
    }
    this->update();
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)

    MSG* msg = reinterpret_cast<MSG*>(message);

    if(msg->message == WM_DEVICECHANGE)
    {
        updateTable();
        return true;
    }

    return false;
}

void Widget::timerEvent(QTimerEvent *)
{
    updateTable();
}


void Widget::updateUsbDevices()
{
    usbDevices.clear();

    QMap<QString, QString> instanceIdToLetterMap = getInstanceIdToLetterMap();
    HDEVINFO info = SetupDiGetClassDevs(&GUID_DEVCLASS_WPD, NULL, NULL, DIGCF_PRESENT);

    SP_DEVINFO_DATA                     DeviceInfoData;
    DeviceInfoData.cbSize             = sizeof(SP_DEVINFO_DATA);

//    BOOL SetupDiGetDeviceRegistryProperty(
//      _In_      HDEVINFO         DeviceInfoSet,
//      _In_      PSP_DEVINFO_DATA DeviceInfoData,
//      _In_      DWORD            Property,
//      _Out_opt_ PDWORD           PropertyRegDataType,
//      _Out_opt_ PBYTE            PropertyBuffer,
//      _In_      DWORD            PropertyBufferSize,
//      _Out_opt_ PDWORD           RequiredSize
//    );

//    BOOL SetupDiGetDeviceInstanceId(
//      _In_      HDEVINFO         DeviceInfoSet,
//      _In_      PSP_DEVINFO_DATA DeviceInfoData,
//      _Out_opt_ PTSTR            DeviceInstanceId,
//      _In_      DWORD            DeviceInstanceIdSize,
//      _Out_opt_ PDWORD           RequiredSize
//    );

    DWORD   PropertyRegDataType;
    BYTE    PropertyBuffer[1024] = {0};
    DWORD   PropertyBufferSize = 1024;
    DWORD   RequiredSize;

    DWORD   i = 0;

    while(SetupDiEnumDeviceInfo(info, i++, &DeviceInfoData))
    {
        UsbDevice * usbDevice = new UsbDevice(this);
//        qDebug () << "DeviceInfoData.DevInst = " << DeviceInfoData.DevInst << endl;

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_DEVICEDESC,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);

        usbDevice->setDeviceDesc(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_DEVICEDESC = " << usbDevice->getDeviceDesc() << endl;

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_FRIENDLYNAME,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);
        usbDevice->setFriendlyName(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_FRIENDLYNAME = " << usbDevice->getFriendlyName() << endl;

        SetupDiGetDeviceInstanceId(info,
                                   &DeviceInfoData,
                                   (wchar_t *)PropertyBuffer,
                                   PropertyBufferSize,
                                   &RequiredSize);

        QString instanceId = QString::fromWCharArray((wchar_t *)PropertyBuffer);

//        qDebug() << "InstanceID  = " << instanceId << endl;
        instanceId = instanceId.mid(0, instanceId.lastIndexOf('#'));
//        qDebug() << "InstanceID = " << instanceId << endl;
        QString letter = instanceIdToLetterMap[instanceId.mid(instanceId.indexOf("_??_"))];
        usbDevice->setLetter(letter);

        instanceId = instanceId.mid(instanceId.indexOf("#"));
        instanceId.replace("#", "\\");
//        qDebug() << "InstanceID = " << instanceId << endl;
        usbDevice->setInstanceID(instanceId);

        if(!letter.isEmpty())
        {
            _ULARGE_INTEGER TotalNumberOfBytes;
            _ULARGE_INTEGER TotalNumberOfFreeBytes;

            if(!GetDiskFreeSpaceEx((wchar_t *)letter.append("\\").utf16(), 0, &TotalNumberOfBytes, &TotalNumberOfFreeBytes))
            {
                qDebug() << "Ошибка получения информации о памяти. Номер ошибки: " << GetLastError();
                usbDevice->setFreeSpace(0);
                usbDevice->setTotalSpace(0);
                usbDevice->setBusySpace(0);
            }
            usbDevice->setFreeSpace(TotalNumberOfFreeBytes.QuadPart);
            usbDevice->setTotalSpace(TotalNumberOfBytes.QuadPart);
            usbDevice->setBusySpace(TotalNumberOfBytes.QuadPart - TotalNumberOfFreeBytes.QuadPart);

//            qDebug() << "diskSpace = " << TotalNumberOfBytes.QuadPart / (1024 * 1024)       << " MB"  << endl
//                     << "freeSpace = " << TotalNumberOfFreeBytes.QuadPart / (1024 * 1024)   << " MB" <<  endl;
        }

        usbDevices.append(usbDevice);
    }
    SetupDiDestroyDeviceInfoList(info);
}

QMap<QString, QString> Widget::getInstanceIdToLetterMap()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\SYSTEM\\MountedDevices", QSettings::NativeFormat);
    QStringList keys = settings.childKeys();
    QStringList letters;

    foreach (QString s, keys) {
        if(s.contains("DosDevices"))
            letters.append(s.right(2));
    }

//    qDebug() << letters << endl;

    QMap<QString, QString> map;

    foreach (QString s, letters)
    {
        HKEY fKey;
        DWORD pcbData = 1024;
        wchar_t pvData[1024];

        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\MountedDevices", 0, KEY_READ, &fKey))
                qDebug() << "Ошибка RegOpenKeyEx " << GetLastError();

        QString str = "\\DosDevices\\";

        if(RegQueryValueEx(fKey, (wchar_t *)str.append(s).utf16(), NULL, NULL, (LPBYTE)pvData, &pcbData))
            qDebug() << "Ошибка RegQueryValueEx " << GetLastError();

        QString mountDevice = QString::fromWCharArray((wchar_t *)pvData).toUpper();

//        qDebug() << "mountDevice = " << mountDevice << endl;

        map[mountDevice.mid(0, mountDevice.lastIndexOf('#'))] = s;
        RegCloseKey(fKey);
    }
//    qDebug() << map << endl;

    return map;
}

void Widget::ejectDevice()
{

    HDEVINFO info = SetupDiGetClassDevs(&GUID_DEVCLASS_DISKDRIVE, NULL, NULL, DIGCF_PRESENT);

    SP_DEVINFO_DATA                     DeviceInfoData;
    DeviceInfoData.cbSize             = sizeof(SP_DEVINFO_DATA);

    BYTE    PropertyBuffer[1024] = {0};
    DWORD   PropertyBufferSize = 1024;
    DWORD   RequiredSize;

    DWORD   i = 0;

    while(SetupDiEnumDeviceInfo(info, i++, &DeviceInfoData))
    {
        SetupDiGetDeviceInstanceId(info,
                                   &DeviceInfoData,
                                   (wchar_t *)PropertyBuffer,
                                   PropertyBufferSize,
                                   &RequiredSize);

        QString instanceId = QString::fromWCharArray((wchar_t *)PropertyBuffer);

        if(instanceId.contains(usbDevices.at(pTable->currentRow())->getInstanceID()))
        {
            ULONG problem = 0;
            ULONG status = 0;
            DEVINST devInst = DeviceInfoData.DevInst;

            forever {
                CM_Get_DevNode_Status(&status, &problem, devInst, NULL);

                if(status & DN_REMOVABLE)
                    break;
                if(CM_Get_Parent(&devInst, devInst, 0) != CR_SUCCESS)
                {
                    SetupDiDestroyDeviceInfoList(info);
                    qDebug() << "Нельзя извлечь данное устройство." << endl;
                    return;
                }
            }
            int result = CM_Request_Device_Eject(devInst, NULL, NULL, 1024, 0);
            if(result == CR_SUCCESS)
            {
                qDebug() << "Устройство успешно отключено." << endl;
            }
            else
            {
                qDebug() << "Ошибка отлючения устройства. Номер ошибки: "<< result << endl;
            }
            break;
        }
    }
    SetupDiDestroyDeviceInfoList(info);
}
