#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::pressed, this, &Widget::getFun);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::getFun()
{
    info = SetupDiGetClassDevs(&GUID_DEVCLASS_WPD, NULL, NULL, DIGCF_PRESENT);

    SP_DEVINFO_DATA             DeviceInfoData;

//    BOOL SetupDiGetDeviceRegistryProperty(
//      _In_      HDEVINFO         DeviceInfoSet,
//      _In_      PSP_DEVINFO_DATA DeviceInfoData,
//      _In_      DWORD            Property,
//      _Out_opt_ PDWORD           PropertyRegDataType,
//      _Out_opt_ PBYTE            PropertyBuffer,
//      _In_      DWORD            PropertyBufferSize,
//      _Out_opt_ PDWORD           RequiredSize
//    );

    DWORD           PropertyRegDataType;
    qint8            PropertyBuffer[1024] = {0};
    DWORD           PropertyBufferSize = 1024;
    DWORD           RequiredSize;

//    SP_DEVICE_INTERFACE_DATA    DeviceInterfaceData;

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD i = 0;
    while(SetupDiEnumDeviceInfo(info, i++, &DeviceInfoData))
    {
        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_DEVICEDESC,
                                         &PropertyRegDataType,
                                         (PBYTE)PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);

        qDebug() << "PropertyRegDataType         = " << PropertyRegDataType     << endl <<
                    "PropertyBuffer[1024]        = " << QString::fromWCharArray((wchar_t *)PropertyBuffer)         << endl <<
                    "RequiredSize                = " << RequiredSize            << endl;

        SetupDiGetDeviceRegistryProperty(info,
                                         &DeviceInfoData,
                                         SPDRP_FRIENDLYNAME,
                                         &PropertyRegDataType,
                                         (PBYTE)PropertyBuffer,
                                         PropertyBufferSize,
                                         &RequiredSize);

        qDebug() << "PropertyRegDataType         = " << PropertyRegDataType     << endl <<
                    "PropertyBuffer[1024]        = " << QString::fromWCharArray((wchar_t *)PropertyBuffer)         << endl <<
                    "RequiredSize                = " << RequiredSize            << endl;

    }



}
