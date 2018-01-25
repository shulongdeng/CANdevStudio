#ifndef CANSIGNALCODER_P_H
#define CANSIGNALCODER_P_H

#include <QtCore/QObject>
#include <memory>
#include "cansignalcoder.h"

class CanSignalCoder;

class CanSignalCoderPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalCoder)

public:
    CanSignalCoderPrivate(CanSignalCoder* q, CanSignalCoderCtx&& ctx = CanSignalCoderCtx());
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);
    void decodeFrame(const QCanBusFrame& frame);

private:
    void initProps();
    std::pair<QString, QVariant> decodeLeSignal(const CANsignal& sig, const QCanBusFrame& frame);
    std::pair<QString, QVariant> decodeBeSignal(const CANsignal& sig, const QCanBusFrame& frame);

public:
    bool _simStarted{ false };
    CanSignalCoderCtx _ctx;
    std::map<QString, QVariant> _props;

private:
    CanSignalCoder* q_ptr;
    CANmessages_t _messages;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALCODER_P_H
