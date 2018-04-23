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

    typedef std::function<void()> clear_t;
    typedef std::function<void()> dockUndock_t;
    typedef std::function<void()> filter_t;

    virtual void setClearCbk(const clear_t& cb) = 0;
    virtual void setDockUndockCbk(const dockUndock_t& cb) = 0;
    virtual void setFilterCbk(const filter_t& cb) = 0;

    virtual QWidget* mainWidget() = 0;
    virtual void initTableView(QAbstractItemModel& tvModel) = 0;
    virtual void newRowAdded() = 0;
};

#endif // CANSIGNALVIEWERGUIINT_H
