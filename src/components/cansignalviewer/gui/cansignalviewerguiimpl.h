#ifndef CANSIGNALVIEWERGUIIMPL_H
#define CANSIGNALVIEWERGUIIMPL_H

#include "ui_cansignalviewer.h"
#include "cansignalviewerguiint.h"
#include <QWidget>

struct CanSignalViewerGuiImpl : public CanSignalViewerGuiInt {
    CanSignalViewerGuiImpl()
        : _ui(new Ui::CanSignalViewerPrivate)
        , _widget(new QWidget)
    {
        _ui->setupUi(_widget);
    }

    virtual void setClearCbk(const clear_t& cb) override
    {
        QObject::connect(_ui->pbClear, &QPushButton::pressed, cb);
    }

    virtual void setDockUndockCbk(const dockUndock_t& cb) override
    {
        QObject::connect(_ui->pbDockUndock, &QPushButton::toggled, cb);
    }

    virtual void setFilterCbk(const filter_t& cb) override
    {
        QObject::connect(_ui->pbFilter, &QPushButton::toggled, cb);
    }

    virtual QWidget* mainWidget()
    {
        return _widget;
    }

    virtual void initTableView(QAbstractItemModel& tvModel) override
    {
        _ui->tv->setModel(&tvModel);
        _ui->tv->horizontalHeader()->setSectionsMovable(true);
        _ui->tv->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        _ui->tv->setColumnHidden(0, true);

        _ui->tv->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

        //tvModel.setHeaderData(0, Qt::Horizontal, QVariant::fromValue(CRV_ColType::uint_type), Qt::UserRole); // rowID
        //tvModel.setHeaderData(1, Qt::Horizontal, QVariant::fromValue(CRV_ColType::double_type), Qt::UserRole); // time
        //tvModel.setHeaderData(2, Qt::Horizontal, QVariant::fromValue(CRV_ColType::hex_type), Qt::UserRole); // frame ID
        //tvModel.setHeaderData(3, Qt::Horizontal, QVariant::fromValue(CRV_ColType::str_type), Qt::UserRole); // name
        //tvModel.setHeaderData(4, Qt::Horizontal, QVariant::fromValue(CRV_ColType::str_type), Qt::UserRole); // value
    }

    virtual void newRowAdded()
    {
        if (!_ui->pbLock->isChecked()) {
            _ui->tv->scrollToBottom();
        }
    }
private:
    Ui::CanSignalViewerPrivate* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALVIEWERGUIIMPL_H
