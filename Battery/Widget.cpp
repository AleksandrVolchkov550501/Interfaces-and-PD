#include "Widget.h"
#include "ui_Widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&powerInfo, SIGNAL(infoChanged()), this, SLOT(update()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::update()
{
    ui->l_powerType->setText(powerInfo.ACLineStatus() ? "AC" : "Battery");
    if(powerInfo.ACLineStatus())
        ui->l_remTime->setText("-");
    else
        ui->l_remTime->setText(powerInfo.getLifeTime().toString("h часов m минут"));
    ui->progressBar->setValue(powerInfo.BatteryLifePercent());

}

