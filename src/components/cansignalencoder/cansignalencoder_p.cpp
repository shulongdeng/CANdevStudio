#include "cansignalencoder_p.h"
#include <log.h>
#include <QCanBusFrame>

CanSignalEncoderPrivate::CanSignalEncoderPrivate(CanSignalEncoder *q, CanSignalEncoderCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanSignalEncoderPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalEncoderPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalEncoderPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalEncoderPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}

const CANmessages_t::value_type* CanSignalEncoderPrivate::findInDb(uint32_t id)
{
    const auto& el = std::find_if(_messages.begin(), _messages.end(), [this, id](const auto& it) {
        if (id == it.first.id) {
            return true;
        } else {
            return false;
        }
    });

    if (el != _messages.end()) {
        return &*el;
    }

    return nullptr;
}

void CanSignalEncoderPrivate::signalToRaw(const uint32_t id, const CANsignal& sigDesc, const QVariant& sigVal) 
{
    int64_t rawVal = static_cast<int64_t>((sigVal.toDouble() - sigDesc.offset) / sigDesc.factor);
    uint8_t *data = (uint8_t*) _rawCache[id].data();

    if (sigDesc.byteOrder == 0) {
        // little endian
        auto bit = sigDesc.startBit;
        for (int bitpos = 0; bitpos < sigDesc.signalSize; bitpos++) {
            // clear bit first
            data[bit / 8] &= ~(1U << (bit % 8));
            // set bit
            data[bit / 8] |= ((rawVal >> bitpos) & 1U) << (bit % 8);
            bit++;
        }

        if(rawVal < 0) {
            if(sigDesc.valueSigned) {
                // make sure that MSB is set for signed value
                bit--;
                data[bit / 8] |= 1U << (bit % 8);
            } else {
                cds_warn("Processing negative value '{}' for unsinged signal!", rawVal);
            }
        }
    } else {
        // motorola / big endian mode
        cds_warn("Motorola not implemented yet");
        //bit = startBit;
        //for (int bitpos = 0; bitpos < sigSize; bitpos++) {
            //if (data[bit / 8] & (1 << (bit % 8)))
                //result += (1ULL << (sigSize - bitpos - 1));

            //if ((bit % 8) == 0)
                //bit += 15;
            //else
                //bit--;
        //}
    }

    QCanBusFrame frame(id, _rawCache[id]);

    emit q_ptr->sendFrame(frame);
}

void CanSignalEncoderPrivate::encodeSignal(const QString& name, const QVariant& val)
{
    auto nameSplit = name.split('_');

    if (nameSplit.size() < 2) {
        cds_error("Wrong signal name: {}", name.toStdString());
        return;
    }

    uint32_t id = nameSplit[0].toUInt(nullptr, 16);

    auto msgDesc = findInDb(id);
    QString sigName = name.mid(name.indexOf("_") + 1);

    if (!msgDesc) {
        cds_warn("Msg '{:03x}' not found in DB", id);

        return;
    }

    for (auto& sig : msgDesc->second) {
        if (sig.signal_name == sigName.toStdString()) {
            if (_rawCache[id].size() < (int)msgDesc->first.dlc) {
                cds_info("Setting up new cache for {:03x} msg", id);
                // Set chache for the first time
                _rawCache[id].fill(0, msgDesc->first.dlc);
            }

            if (_rawCache[id].size() * 8 >= sig.startBit + sig.signalSize) {

                signalToRaw(id, sig, val);
                return;
            } else {
                cds_error("Payload size ('{}') for signal '{}' is to small. StartBit {}, signalSize {}",
                    _rawCache[id].size() * 8, sig.signal_name, sig.startBit, sig.signalSize);
            }
        }
    }

    cds_warn("Failed to find '{}' signal in '{:03x}' msg", sigName.toStdString(), id);

    return;
}
