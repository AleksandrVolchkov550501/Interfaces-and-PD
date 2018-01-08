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

    updateDiskInfo();
    updateDiskSpace();
    updateATA_and_POI_DMA_Support();
}

DiskInfo::~DiskInfo()
{
    CloseHandle(diskHandle);
}

QString DiskInfo::getModel() const
{
    return model;
}

QString DiskInfo::getFirmware() const
{
    return firmware;
}

QString DiskInfo::getBusType() const
{
    return busType;
}

QString DiskInfo::getSN() const
{
    return SN;
}

QString DiskInfo::getAtaStdSupportString() const
{
    return ataStdSupportString;
}

QString DiskInfo::getUDMAModsString() const
{
    return UDMAModsString;
}

QString DiskInfo::getMDMAModsString() const
{
    return MDMAModsString;
}

QString DiskInfo::getPIOModsString() const
{
    return PIOModsString;
}

quint64 DiskInfo::getFreeSpace() const
{
    return freeSpace;
}

quint64 DiskInfo::getBusySpace() const
{
    return busySpace;
}

quint64 DiskInfo::getTotalSpace() const
{
    return totalSpace;
}

void DiskInfo::updateDiskInfo()
{

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
        qDebug() << "Ошибка получения информации о диске. Номер ошибки: " << GetLastError();
    }

    STORAGE_DEVICE_DESCRIPTOR * deviceDescriptor = static_cast<STORAGE_DEVICE_DESCRIPTOR *> (lpOutBuffer);

    model = (char*)(deviceDescriptor) + deviceDescriptor->ProductIdOffset;
    firmware   = (char*)(deviceDescriptor) + deviceDescriptor->ProductRevisionOffset;
    SN        = (char*)(deviceDescriptor) + deviceDescriptor->SerialNumberOffset;
    busType   = busTypeList[deviceDescriptor->BusType];

    qDebug() << "productID = " << model << endl
             << "version = " << firmware << endl
             << "SN = " << SN << endl
             << "Bus type = " << busType << endl;

    ::operator delete(lpOutBuffer);
}

void DiskInfo::updateDiskSpace()
{
    _ULARGE_INTEGER TotalNumberOfBytes;
    _ULARGE_INTEGER TotalNumberOfFreeBytes;

    if(!GetDiskFreeSpaceExA("c:\\", 0, &TotalNumberOfBytes, &TotalNumberOfFreeBytes))
    {
        qDebug() << "Ошибка получения информации о памяти. Номер ошибки: " << GetLastError();
    }
    freeSpace  = TotalNumberOfFreeBytes.QuadPart;
    totalSpace = TotalNumberOfBytes.QuadPart;
    busySpace = totalSpace - freeSpace;

    qDebug() << "diskSpace = " << TotalNumberOfBytes.QuadPart       << endl
             << "freeSpace = " << TotalNumberOfFreeBytes.QuadPart << endl;
}

void DiskInfo::updateATA_and_POI_DMA_Support()
{
    UCHAR identifyDataBuffer[512 + sizeof(ATA_PASS_THROUGH_EX)] = { 0 };

    ATA_PASS_THROUGH_EX &PTE = *(ATA_PASS_THROUGH_EX *)identifyDataBuffer;
    PTE.Length = sizeof(PTE);
    PTE.TimeOutValue = 10;
    PTE.DataTransferLength = 512;
    PTE.DataBufferOffset = sizeof(ATA_PASS_THROUGH_EX);
    PTE.AtaFlags = ATA_FLAGS_DATA_IN;

    IDEREGS *ideRegs = (IDEREGS *)PTE.CurrentTaskFile;
    ideRegs->bCommandReg = 0xEC;

    if (!DeviceIoControl(diskHandle,
                         IOCTL_ATA_PASS_THROUGH,
                         &PTE,
                         sizeof(identifyDataBuffer),
                         &PTE,
                         sizeof(identifyDataBuffer),
                         NULL,
                         NULL))
    {
        qDebug() << "Ошибка IOCTL_ATA_PASS_THROUGH. Код ошибки: " <<  GetLastError() << endl;
        return;
    }

    WORD *data = (WORD *)(identifyDataBuffer + sizeof(ATA_PASS_THROUGH_EX));

    WORD ataStdSupport = data[80];

    qDebug() << "ataStdSupport = " << ataStdSupport << endl;

    if(ataStdSupport & 32)
        ataStdSupportString.append("ATA/ATAPI-5, ");
    if(ataStdSupport & 64)
        ataStdSupportString.append("ATA/ATAPI-6, ");
    if(ataStdSupport & 128)
        ataStdSupportString.append("ATA/ATAPI-7, ");
    if(ataStdSupport & 256)
        ataStdSupportString.append("ATA8-ACS, ");
    if(ataStdSupport & 512)
        ataStdSupportString.append("ACS-2, ");
    ataStdSupportString.chop(2);

    qDebug() << "ataStdSupportString = " << ataStdSupportString << endl;

    WORD UDMAMods = data[88];

    UDMAModsString.append("Ultra DMA mode ");
    if(UDMAMods & 64)
        UDMAModsString.append("6");
    else if(UDMAMods & 32)
        UDMAModsString.append("5");
    else if(UDMAMods & 16)
        UDMAModsString.append("4");
    else if(UDMAMods & 8)
        UDMAModsString.append("3");
    else if(UDMAMods & 4)
        UDMAModsString.append("2");
    else if(UDMAMods & 2)
        UDMAModsString.append("1");
    else if(UDMAMods & 1)
        UDMAModsString.append("0");
    UDMAModsString.append(" and below are supported");

    qDebug() << "UDMAModsString = " << UDMAModsString << endl;

    WORD MDMAMods = data[63];

    MDMAModsString.append("Multiword DMA mode ");
    if(MDMAMods & 4)
        MDMAModsString.append("2");
    else if(MDMAMods & 2)
        MDMAModsString.append("1");
    else if(MDMAMods & 1)
        MDMAModsString.append("0");
    MDMAModsString.append(" and below are supported");

    qDebug() << "MDMAModsString = " << MDMAModsString << endl;

    WORD PIOMods = data[64];

    PIOModsString.append("PIO mode ");
    if(PIOMods & 2)
        PIOModsString.append("4");
    else if(PIOMods & 1)
        PIOModsString.append("3");
    PIOModsString.append(" and below are supported");

    qDebug() << "POIModsString = " << PIOModsString << endl;
}
