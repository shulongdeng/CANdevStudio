#ifndef CANSIGNALSENDERGUIIMPL_H
#define CANSIGNALSENDERGUIIMPL_H

#include "cansignalsenderguiint.h"
#include "ui_cansignalsender.h"
#include <QStandardItemModel>
#include <QWidget>
#include <log.h>

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

    virtual void initTv(QStandardItemModel& tvModel) override
    {
        _model = &tvModel;
        _ui->tv->setModel((QAbstractItemModel*)&tvModel);
        _ui->tv->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        _ui->tv->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
        _ui->tv->horizontalHeader()->setSectionsMovable(true);
    }

    virtual void setDockUndockCbk(const dockUndock_t& cb) override
    {
        QObject::connect(_ui->pbDockUndock, &QPushButton::toggled, cb);
    }

    virtual void setAddCbk(const add_t& cb) override
    {
        QObject::connect(_ui->pbAdd, &QPushButton::pressed, cb);
    }

    virtual void setSendCbk(const send_t& cbk)
    {
        _sendCbk = cbk;
    }

    virtual void addRow()
    {
        if (!_model) {
            cds_error("_model is NULL");
            return;
        }

        QList<QStandardItem*> list{ new QStandardItem(), new QStandardItem(), new QStandardItem() };

        _model->appendRow(list);

        QPushButton* bt = new QPushButton("Send");
        bt->setProperty("type", "nlmItem");
        bt->setFlat(true);

        auto itemNdx = _model->index(_model->rowCount() - 1, _model->columnCount() - 1);
        _ui->tv->setIndexWidget(itemNdx, bt);

        QObject::connect(bt, &QPushButton::pressed, [list, this] {
            if (_sendCbk) {
                if (list[0]->data(Qt::DisplayRole).toString().length() > 0
                    && list[1]->data(Qt::DisplayRole).toString().length() > 0
                    && list[2]->data(Qt::DisplayRole).toString().length() > 0) {

                    _sendCbk(list[0]->data(Qt::DisplayRole).toString(), list[1]->data(Qt::DisplayRole).toString(),
                        list[2]->data(Qt::DisplayRole));
                }
            }
        });
    }

private:
    send_t _sendCbk{ nullptr };
    QStandardItemModel* _model;
    Ui::CanSignalSenderPrivate* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALSENDERGUIIMPL_H
