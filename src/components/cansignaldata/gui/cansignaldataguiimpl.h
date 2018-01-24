#ifndef CANSIGNALDATAGUIIMPL_H
#define CANSIGNALDATAGUIIMPL_H

#include <QWidget>
#include "cansignaldataguiint.h"
#include "ui_cansignaldata.h"

struct CanSignalDataGuiImpl : public CanSignalDataGuiInt {
    CanSignalDataGuiImpl()
        : _ui(new Ui::CanSignalData)
        , _widget(new QWidget)
    {
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

private:
    Ui::CanSignalData* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALDATAGUIIMPL_H
