#include "cansignalcoder_p.h"
#include <QCanBusFrame>
#include <algorithm>
#include <log.h>

CanSignalCoderPrivate::CanSignalCoderPrivate(CanSignalCoder* q, CanSignalCoderCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanSignalCoderPrivate::initProps()
{
    for (const auto& p : _supportedProps) {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalCoderPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalCoderPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalCoderPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}

void CanSignalCoderPrivate::decodeFrame(const QCanBusFrame& frame)
{
    const auto& el = std::find_if(_messages.begin(), _messages.end(), [this, &frame](const auto& it) {
        if (frame.frameId() == it.first.id) {
            return true;
        } else {
            return false;
        }
    });

    if (el != _messages.end()) {
        cds_info("Frame '{:x}' exists in DB", el->first.id);

        for(auto &sig: el->second) {
            if(sig.byteOrder) {
                // Big endian
                // StartBit - position of the most-significant bit.
            } else {
                // Little endian
                // StartBit - position of the least-significant bit.
            }
        }

    } else {
        cds_info("Frame '{:x}' does not exist in DB", frame.frameId());
    }
}

std::pair<QString, QVariant> CanSignalCoderPrivate::decodeLeSignal(const CANsignal& sig, const QCanBusFrame& frame)
{
    return { };
}

std::pair<QString, QVariant> CanSignalCoderPrivate::decodeBeSignal(const CANsignal& sig, const QCanBusFrame& frame)
{
    //uint64_t val = 0;
    //const uint8_t startNdx = sig.startBit/8;
    //const uint8_t startBitmask = 0xff >> (sig.startBit % 8);
    //const uint8_t endNdx = sig.startBit/8 + sig.signalSize/8;
    //const uint8_t endBit = (sig.startBit + sig.signalSize) % 8;

    //if(startNdx >= frame.payload().size() || endNdx >= frame.payload().size()) {
        //cds_error("Wrong signal indexes, start {}, end {}, payload {}", startNdx, endNdx, frame.payload().size());
        //return { };
    //}

    //if(startNdx == endNdx) {
        //if(
    //} else {
    //}


    //val = frame.payload()[startNdx] &
    //for(uint8_t i = startNdx; i < endNdx; ++i) {
    //}

    return { };
}

