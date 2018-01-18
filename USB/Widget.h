#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QByteArray>
#include "Windows.h"
#include "devguid.h"
#include "SetupAPI.h"
#include "Dbt.h"
#include "devpkey.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    HDEVINFO info;

public slots:
    void getFun();
};

#endif // WIDGET_H
