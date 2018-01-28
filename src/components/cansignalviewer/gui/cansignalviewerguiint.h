#ifndef CANSIGNALVIEWERGUIINT_H
#define CANSIGNALVIEWERGUIINT_H

#include <Qt>
#include <functional>

class QWidget;
class QAbstractItemModel;

struct CanSignalViewerGuiInt {
    virtual ~CanSignalViewerGuiInt()
    {
    }

    virtual QWidget* mainWidget() = 0;
    virtual void initTableView(QAbstractItemModel& tvModel) = 0;
};

#endif // CANSIGNALVIEWERGUIINT_H
