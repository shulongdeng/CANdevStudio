#ifndef CANSIGNALVIEWER_P_H
#define CANSIGNALVIEWER_P_H

#include "gui/cansignalviewerguiimpl.h"
#include "cansignalviewer.h"
#include <QtCore/QObject>
#include <memory>
#include <QStandardItemModel>
#include <QElapsedTimer>

class CanSignalViewer;

class CanSignalViewerPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalViewer)

public:
    CanSignalViewerPrivate(CanSignalViewer* q, CanSignalViewerCtx&& ctx = CanSignalViewerCtx(new CanSignalViewerGuiImpl));
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);
    void addSignal(const QString& id, const QString& name, const QString& value, const QString& dir);
    void clear();

private:
    void initProps();

public:
    bool _simStarted{ false };
    CanSignalViewerCtx _ctx;
    CanSignalViewerGuiInt& _ui;
    bool _docked{ true };
    std::map<QString, QVariant> _props;
    QStandardItemModel _tvModel;

private:
    QElapsedTimer _timer;
    uint64_t _rowId {1};
    QStringList _columns {"rowId", "Time", "Id", "Dir", "Signal name", "Signal value"};
    CanSignalViewer* q_ptr;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALVIEWER_P_H
