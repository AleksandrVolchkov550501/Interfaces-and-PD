#include "DiskInfo.h"
#include <QDebug>

DiskInfo::DiskInfo(QObject *parent) : QObject(parent)
{
    busTypeList << "UNKNOWN" << "SCSI" << "ATAPI" << "ATA" << "ONE_TREE_NINE_FOUR" << "SSA" << "FIBRE"
                << "USB" << "RAID" << "ISCSI" << "SAS" << "SATA" << "SD" << "MMC";

    storageProtertyQuery.QueryType = PropertyStandardQuery;
    storageProtertyQuery.PropertyId = StorageDeviceProperty;

    diskHandle = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (diskHandle == INVALID_HANDLE_VALUE) {
        qDebug() << "Ошибка: " << GetLastError();
    }

    STORAGE_DESCRIPTOR_HEADER deviceDescriptorHeader;
    ZeroMemory(&deviceDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER));
    if(!DeviceIoControl(diskHandle,
                    IOCTL_STORAGE_QUERY_PROPERTY,
                    &storageProtertyQuery,
                    sizeof(storageProtertyQuery),
                    &deviceDescriptorHeader,
                    sizeof(STORAGE_DESCRIPTOR_HEADER),
                    NULL,
                    NULL))
    {
        qDebug() << "Ошибка получения STORAGE_DESCRIPTOR_HEADER: " << GetLastError();
    }

    LPVOID lpOutBuffer = ::operator new (deviceDescriptorHeader.Size) ;
    ZeroMemory(lpOutBuffer, deviceDescriptorHeader.Size);

    if(!DeviceIoControl(diskHandle,
                    IOCTL_STORAGE_QUERY_PROPERTY,
                    &storageProtertyQuery,
                    sizeof(storageProtertyQuery),
                    lpOutBuffer,
                    deviceDescriptorHeader.Size,
                    NULL,
                    NULL))
    {
        qDebug() << "Ошибка: " << GetLastError();
    }

    STORAGE_DEVICE_DESCRIPTOR * deviceDescriptor = static_cast<STORAGE_DEVICE_DESCRIPTOR *> (lpOutBuffer);

    productId = (char*)(deviceDescriptor) + deviceDescriptor->ProductIdOffset;
    version   = (char*)(deviceDescriptor) + deviceDescriptor->ProductRevisionOffset;
    SN        = (char*)(deviceDescriptor) + deviceDescriptor->SerialNumberOffset;
    busType   = busTypeList[deviceDescriptor->BusType];

    qDebug() << "productID = " << productId << ", " << endl
             << "version = " << version << endl
             << "SN = " << SN << endl
             << "Bus type = " << busType << endl;

    ::operator delete(lpOutBuffer);

}

DiskInfo::~DiskInfo()
{
    CloseHandle(diskHandle);
}
