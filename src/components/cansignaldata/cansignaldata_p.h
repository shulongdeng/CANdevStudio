#ifndef CANSIGNALDATA_P_H
#define CANSIGNALDATA_P_H

#include <QtCore/QObject>
#include <memory>
#include "gui/cansignaldataguiimpl.h"
#include "cansignaldata.h"

class CanSignalData;

class CanSignalDataPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalData)

public:
    CanSignalDataPrivate(CanSignalData* q, CanSignalDataCtx&& ctx = CanSignalDataCtx(new CanSignalDataGuiImpl));
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);

private:
    void initProps();

public:
    bool _simStarted{ false };
    CanSignalDataCtx _ctx;
    CanSignalDataGuiInt& _ui;
    bool _docked{ true };
    std::map<QString, QVariant> _props;

private:
    CanSignalData* q_ptr;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALDATA_P_H
