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
        _ui->setupUi(_widget);
        _widget->setMinimumSize(_ui->tv->minimumSize());
    }

    virtual QWidget* mainWidget() override
    {
        return _widget;
    }

    virtual void initTableView(QAbstractItemModel& tvModel) override
    {
        _ui->tv->setModel(&tvModel);
        _ui->tv->horizontalHeader()->setSectionsMovable(true);
        _ui->tv->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        _ui->tv->setColumnHidden(0, true);

        //tvModel.setHeaderData(0, Qt::Horizontal, QVariant::fromValue(CRV_ColType::uint_type), Qt::UserRole); // rowID
        //tvModel.setHeaderData(1, Qt::Horizontal, QVariant::fromValue(CRV_ColType::double_type), Qt::UserRole); // time
        //tvModel.setHeaderData(2, Qt::Horizontal, QVariant::fromValue(CRV_ColType::hex_type), Qt::UserRole); // frame ID
        //tvModel.setHeaderData(3, Qt::Horizontal, QVariant::fromValue(CRV_ColType::str_type), Qt::UserRole); // direction
        //tvModel.setHeaderData(4, Qt::Horizontal, QVariant::fromValue(CRV_ColType::uint_type), Qt::UserRole); // length
        //tvModel.setHeaderData(5, Qt::Horizontal, QVariant::fromValue(CRV_ColType::str_type), Qt::UserRole); // data
    }


private:
    Ui::CanSignalData* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALDATAGUIIMPL_H
