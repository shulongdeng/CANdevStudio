#ifndef CANSIGNALSENDERGUIIMPL_H
#define CANSIGNALSENDERGUIIMPL_H

#include <QWidget>
#include "cansignalsenderguiint.h"
#include "ui_cansignalsender.h"

struct CanSignalSenderGuiImpl : public CanSignalSenderGuiInt {
    CanSignalSenderGuiImpl()
        : _ui(new Ui::CanSignalSender)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

private:
    Ui::CanSignalSender* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALSENDERGUIIMPL_H
