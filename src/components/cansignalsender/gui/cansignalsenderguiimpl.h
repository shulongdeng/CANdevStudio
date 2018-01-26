#ifndef CANSIGNALSENDERGUIIMPL_H
#define CANSIGNALSENDERGUIIMPL_H

#include <QWidget>
#include "cansignalsenderguiint.h"
#include "ui_cansignalsender.h"

struct CanSignalSenderGuiImpl : public CanSignalSenderGuiInt {
    CanSignalSenderGuiImpl()
        : _ui(new Ui::CanSignalSenderPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

    virtual void initTv(QAbstractItemModel& tvModel) override
    {
        _ui->tv->setModel(&tvModel);
        _ui->tv->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        _ui->tv->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        _ui->tv->horizontalHeader()->setSectionsMovable(true);
    }


private:
    Ui::CanSignalSenderPrivate* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALSENDERGUIIMPL_H
