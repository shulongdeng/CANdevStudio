#ifndef CANSIGNALSENDERGUIIMPL_H
#define CANSIGNALSENDERGUIIMPL_H

#include "cansignalsenderguiint.h"
#include "ui_cansignalsender.h"
#include <QComboBox>
#include <QLineEdit>
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

    virtual void addRow(const std::map<uint32_t, QStringList>& sigNames) override
    {
        _sigNames = &sigNames;

        if (!_model) {
            cds_error("_model is NULL");
            return;
        }

        QList<QStandardItem*> list{ new QStandardItem(), new QStandardItem(), new QStandardItem() };

        _model->appendRow(list);

        QPushButton* bt = new QPushButton("Send");
        bt->setProperty("type", "nlmItem");
        bt->setFlat(true);

        QComboBox* sigCmb = new QComboBox();

        QComboBox* idCmb = new QComboBox();

        QObject::connect(idCmb, &QComboBox::currentTextChanged, [this, sigCmb](const QString& text) {
            uint32_t id = text.toUInt(nullptr, 16);
            sigCmb->clear();

            if (_sigNames->count(id)) {
                sigCmb->addItems(_sigNames->at(id));
            } else {
                cds_error("No signals for selected id 0x{:03x}", id);
            }
        });

        for (const auto& msg : sigNames) {
            idCmb->addItem(fmt::format("0x{:03x}", msg.first).c_str());
        }

        QLineEdit* le = new QLineEdit();

        QObject::connect(bt, &QPushButton::pressed, [idCmb, sigCmb, le, this] {
            if (_sendCbk) {
                if (idCmb->currentText().length() > 0 && sigCmb->currentText().length() > 0
                    && le->text().length() > 0) {

                    _sendCbk(idCmb->currentText(), sigCmb->currentText(), QVariant(le->text()));
                }
            }
        });

        auto idNdx = _model->index(_model->rowCount() - 1, 0);
        _ui->tv->setIndexWidget(idNdx, idCmb);

        auto sigNdx = _model->index(_model->rowCount() - 1, 1);
        _ui->tv->setIndexWidget(sigNdx, sigCmb);

        auto leNdx = _model->index(_model->rowCount() - 1, 2);
        _ui->tv->setIndexWidget(leNdx, le);

        auto pbNdx = _model->index(_model->rowCount() - 1, _model->columnCount() - 1);
        _ui->tv->setIndexWidget(pbNdx, bt);
    }

private:
    send_t _sendCbk{ nullptr };
    QStandardItemModel* _model;
    const std::map<uint32_t, QStringList>* _sigNames;
    Ui::CanSignalSenderPrivate* _ui;
    QWidget* _widget;
};

#endif // CANSIGNALSENDERGUIIMPL_H
