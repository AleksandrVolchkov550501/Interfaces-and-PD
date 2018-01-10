#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "PowerInfo.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void update();

private:
    Ui::Widget *ui;
    PowerInfo powerInfo;
};

#endif // WIDGET_H
