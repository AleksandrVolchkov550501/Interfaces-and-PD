#include "Widget.h"
#include "ui_Widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->l_modelText->setText(diskInfo.getModel());
    ui->l_firmwareText->setText(diskInfo.getFirmware());
    ui->l_interfaceText->setText(diskInfo.getBusType());
    ui->l_snText->setText(diskInfo.getSN());

    ui->l_freeSpaceText->setText(QString::number(diskInfo.getFreeSpace()/(1024*1024)) + " MB");
    ui->l_totalSpaceText->setText(QString::number(diskInfo.getTotalSpace()/(1024*1024)) + " МВ");

    ui->progressBar->setValue(100 * diskInfo.getBusySpace() / diskInfo.getTotalSpace());

    ui->l_ataStd->setText(diskInfo.getAtaStdSupportString());
    ui->l_MDMA->setText(diskInfo.getMDMAModsString());
    ui->l_PIO->setText(diskInfo.getPIOModsString());
    ui->l_UDMA->setText(diskInfo.getUDMAModsString());

}

Widget::~Widget()
{
    delete ui;
}
