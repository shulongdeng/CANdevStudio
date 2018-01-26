#ifndef CANSIGNALSENDERGUIINT_H
#define CANSIGNALSENDERGUIINT_H

#include <Qt>
#include <functional>

class QWidget;
class QAbstractItemModel;

struct CanSignalSenderGuiInt {
    virtual ~CanSignalSenderGuiInt()
    {
    }

    virtual QWidget* mainWidget() = 0;
    virtual void initTv(QAbstractItemModel& tvModel) = 0;
};

#endif // CANSIGNALSENDERGUIINT_H
