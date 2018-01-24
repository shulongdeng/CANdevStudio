#ifndef CANSIGNALDATAGUIINT_H
#define CANSIGNALDATAGUIINT_H

#include <Qt>
#include <functional>

class QWidget;
class QAbstractItemModel;

struct CanSignalDataGuiInt {
    virtual ~CanSignalDataGuiInt()
    {
    }

    virtual QWidget* mainWidget() = 0;
    virtual void initTableView(QAbstractItemModel& tvModel) = 0;
};

#endif // CANSIGNALDATAGUIINT_H
