#ifndef CANSIGNALDECODER_P_H
#define CANSIGNALDECODER_P_H

#include "cansignaldecoder.h"
#include <QtCore/QObject>
#include <memory>
#include <cantypes.hpp>

class CanSignalDecoder;
class QCanBusFrame;

class CanSignalDecoderPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanSignalDecoder)

public:
    CanSignalDecoderPrivate(CanSignalDecoder* q, CanSignalDecoderCtx&& ctx = CanSignalDecoderCtx());
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);
    void decodeFrame(const QCanBusFrame& frame);

private:
    void initProps();
    int64_t rawToSignal(const uint8_t *data, int startBit, int sigSize, bool littleEndian, bool isSigned);
    const CANmessages_t::value_type* findInDb(uint32_t id); 

public:
    bool _simStarted{ false };
    CanSignalDecoderCtx _ctx;
    std::map<QString, QVariant> _props;

private:
    CanSignalDecoder* q_ptr;
    CANmessages_t _messages;
    const QString _nameProperty = "name";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}}
    };
};

#endif // CANSIGNALDECODER_P_H
