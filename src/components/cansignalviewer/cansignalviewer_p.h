#ifndef CANSIGNALVIEWER_P_H
#define CANSIGNALVIEWER_P_H

#include "gui/cansignalviewerguiimpl.h"
#include "cansignalviewer.h"
#include <QtCore/QObject>
#include <memory>

class CanSignalViewer;

class CanSignalViewerPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalViewer)

public:
    CanSignalViewerPrivate(CanSignalViewer* q, CanSignalViewerCtx&& ctx = CanSignalViewerCtx(new CanSignalViewerGuiImpl));
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);

private:
    void initProps();

public:
    bool _simStarted{ false };
    CanSignalViewerCtx _ctx;
    CanSignalViewerGuiInt& _ui;
    bool _docked{ true };
    std::map<QString, QVariant> _props;

private:
    CanSignalViewer* q_ptr;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALVIEWER_P_H
