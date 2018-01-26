#ifndef CANSIGNALSENDERGUIINT_H
#define CANSIGNALSENDERGUIINT_H

#include <Qt>
#include <functional>

class QWidget;
class QStandardItemModel;

struct CanSignalSenderGuiInt {
    using dockUndock_t = std::function<void()>;
    using add_t = std::function<void()>;
    using send_t = std::function<void(const QString&, const QString&, const QVariant&)>;

    virtual ~CanSignalSenderGuiInt()
    {
    }

    virtual QWidget* mainWidget() = 0;
    virtual void initTv(QStandardItemModel& tvModel) = 0;
    virtual void setAddCbk(const add_t& cb) = 0;
    virtual void setDockUndockCbk(const dockUndock_t& cb) = 0;
    virtual void setSendCbk(const send_t& cbk) = 0;
    virtual void addRow() = 0;
};

#endif // CANSIGNALSENDERGUIINT_H
