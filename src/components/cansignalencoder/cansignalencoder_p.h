#ifndef CANSIGNALENCODER_P_H
#define CANSIGNALENCODER_P_H

#include "cansignalencoder.h"
#include <QtCore/QObject>
#include <memory>

class CanSignalEncoder;

class CanSignalEncoderPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalEncoder)

public:
    CanSignalEncoderPrivate(CanSignalEncoder* q, CanSignalEncoderCtx&& ctx = CanSignalEncoderCtx());
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);
    void encodeSignal(const QString& name, const QVariant& val);

private:
    void initProps();
    const CANmessages_t::value_type* findInDb(uint32_t id); 
    void signalToRaw(const uint32_t id, const CANsignal& sigDesc, const QVariant& sigVal);

public:
    bool _simStarted{ false };
    CanSignalEncoderCtx _ctx;
    std::map<QString, QVariant> _props;

private:
    CanSignalEncoder* q_ptr;
    CANmessages_t _messages;
    std::map<uint32_t, QByteArray> _rawCache;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALENCODER_P_H
