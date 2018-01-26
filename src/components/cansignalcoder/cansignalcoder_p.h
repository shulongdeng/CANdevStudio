#ifndef CANSIGNALCODER_P_H
#define CANSIGNALCODER_P_H

#include <QtCore/QObject>
#include <memory>
#include "cansignalcoder.h"
#include <cantypes.hpp>

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
    void encodeSignal(const QString& name, const QVariant& val);

private:
    void initProps();
    int64_t rawToSignal(const uint8_t *data, int startBit, int sigSize, bool littleEndian, bool isSigned);
    void signalToRaw(const uint32_t id, const CANsignal& sigDesc, const QVariant& sigVal);
    const CANmessages_t::value_type* findInDb(uint32_t id); 

public:
    bool _simStarted{ false };
    CanSignalCoderCtx _ctx;
    std::map<QString, QVariant> _props;

private:
    CanSignalCoder* q_ptr;
    CANmessages_t _messages;
    std::map<uint32_t, QByteArray> _rawCache;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALCODER_P_H
