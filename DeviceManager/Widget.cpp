#include "Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    updateDevInfo();
    updateModel();
    QStringList headers;
    headers << "Описание драйвера" << "Производитель" << "HardwareID" << "GUID" << "Драйвер" << "Device Path";
    model->setHorizontalHeaderLabels(headers);

    treeView.setModel(model);

//    QPushButton * pushButton = new QPushButton("Go!", this);
    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(&treeView);
//    layout->addWidget(pushButton);
    this->setLayout(layout);
    this->resize(1800,800);

//    connect(pushButton, &QPushButton::pressed, this, &Widget::fun);
}

Widget::~Widget()
{

}

void Widget::updateModel()
{
    model->deleteLater();

    QList<QString> keys = multiMap.uniqueKeys();
//    qDebug() << "keys = " << keys;
    model = new QStandardItemModel(keys.size(), 10);

    for(int i = 0; i < keys.size(); i++)
    {
        QList<Device *> listDevs = multiMap.values(keys.at(i));
        QModelIndex index = model->index(i, 0);
        model->setData(index, keys.at(i));
        model->insertRows(0, listDevs.size(), index);
        model->insertColumns(0, 10, index);
        for (int nRow = 0; nRow < listDevs.size(); ++nRow) {
                model->setData(model->index(nRow, 0, index), listDevs.at(nRow)->getDriverDesc());
                model->setData(model->index(nRow, 1, index), listDevs.at(nRow)->getManufacturer());
                model->setData(model->index(nRow, 2, index), listDevs.at(nRow)->getHardwareId());
                model->setData(model->index(nRow, 3, index), listDevs.at(nRow)->getGuid());
                model->setData(model->index(nRow, 4, index), listDevs.at(nRow)->getDriverInfPath());
                model->setData(model->index(nRow, 5, index), listDevs.at(nRow)->getDevicePath());
        }
    }
}

void Widget::updateDevInfo()
{
    multiMap.clear();
    HDEVINFO info = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

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

    DWORD       PropertyRegDataType;
    BYTE        PropertyBuffer[1024] = {0};
    DWORD       PropertyBufferSize = 1024;
    DWORD       RequiredSize;
    DEVPROPTYPE dpt = 0;

    DWORD   i = 0;

    while(SetupDiEnumDeviceInfo(info, i++, &DeviceInfoData))
    {
        Device * device = new Device();

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_HARDWAREID,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);
        device->setHardwareId(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_HARDWAREID = " << device->getHardwareId();

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_MFG,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);
        device->setManufacturer(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_MFG = " << device->getManufacturer();

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_CLASS,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);
        device->setDeviceClass(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_CLASS = " << device->getDeviceClass();

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_CLASSGUID,
                                         &PropertyRegDataType,
                                         PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);
        device->setGuid(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << "SPDRP_CLASSGUID = " << device->getGuid() << endl;

        multiMap.insert(device->getDeviceClass(), device);
        SetupDiGetDeviceInstanceId(info,
                                   &DeviceInfoData,
                                   (wchar_t *)PropertyBuffer,
                                   PropertyBufferSize,
                                   &RequiredSize);

        QString instanceId = QString::fromWCharArray((wchar_t *)PropertyBuffer);
        device->setDevicePath(instanceId);

        const DEVPROPKEY devpkey_Device_DriverInfPath = {0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 5};

        SetupDiGetDeviceProperty(info, &DeviceInfoData, &devpkey_Device_DriverInfPath, &dpt, PropertyBuffer, PropertyBufferSize, NULL, 0);
        device->setDriverInfPath(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << device->getDriverInfPath();

        const DEVPROPKEY devpkey_Device_DriverDesc = {0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 4};

        SetupDiGetDeviceProperty(info, &DeviceInfoData, &devpkey_Device_DriverDesc, &dpt, PropertyBuffer, PropertyBufferSize, NULL, 0);
        device->setDriverDesc(QString::fromWCharArray((wchar_t *)PropertyBuffer));
//        qDebug() << device->getDriverDesc();

//        usbDevices.append(usbDevice);
    }
    SetupDiDestroyDeviceInfoList(info);
}
