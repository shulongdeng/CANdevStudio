#include "cansignalcoder_p.h"
#include <QCanBusFrame>
#include <algorithm>
#include <cmath>
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

const CANmessages_t::value_type* CanSignalCoderPrivate::findInDb(uint32_t id)
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

void CanSignalCoderPrivate::decodeFrame(const QCanBusFrame& frame)
{
    auto el = findInDb(frame.frameId());

    if (el) {
        cds_info("Frame '{:x}' exists in DB", el->first.id);
        bool le = false;

        for (auto& sig : el->second) {
            // Check scope

            if ((sig.startBit >= (frame.payload().size() * 8))
                || ((sig.startBit + sig.signalSize - 1) >= (frame.payload().size() * 8))) {

                cds_error("Invalid message size - startBit {}, sigSize {}, payload size {}", sig.startBit,
                    sig.signalSize, frame.payload().size());
            }

            switch (sig.byteOrder) {
            case 0:
                // Little endian
                le = true;
                break;

            case 1:
                // Big endian
                le = false;
                break;

            default:
                cds_error("payload type {} not suppoerted", sig.byteOrder);
                return;
            }

            int64_t value = rawToSignal(
                (const uint8_t*)frame.payload().constData(), sig.startBit, sig.signalSize, le, sig.valueSigned);
            QVariant sigVal;

            if ((std::fmod(sig.factor, 1) == 0.0) && (std::fmod(sig.offset, 1) == 0.0)) {
                // resulting number will be integer
                value = value * sig.factor + sig.offset;
                sigVal.setValue(value);
            } else {
                double fValue = value * sig.factor + sig.offset;
                sigVal.setValue(fValue);
            }

            QString sigName = fmt::format(
                "0x{:03x}{}_{}", frame.frameId(), frame.hasExtendedFrameFormat() ? "x" : "", sig.signal_name)
                                  .c_str();

            emit q_ptr->sendSignal(sigName, sigVal);

            cds_info("Signal: {}, val: {}", sigName.toStdString(), sigVal.toDouble());
        }

    } else {
        cds_info("Frame '{:x}' does not exist in DB", frame.frameId());
    }
}

// coppied form https://github.com/collin80/SavvyCAN
int64_t CanSignalCoderPrivate::rawToSignal(
    const uint8_t* data, int startBit, int sigSize, bool littleEndian, bool isSigned)
{
    int64_t result = 0;
    int bit;

    if (littleEndian) {
        bit = startBit;
        for (int bitpos = 0; bitpos < sigSize; bitpos++) {
            if (data[bit / 8] & (1 << (bit % 8)))
                result += (1ULL << bitpos);
            bit++;
        }
    } else // motorola / big endian mode
    {
        bit = startBit;
        for (int bitpos = 0; bitpos < sigSize; bitpos++) {
            if (data[bit / 8] & (1 << (bit % 8)))
                result += (1ULL << (sigSize - bitpos - 1));

            if ((bit % 8) == 0)
                bit += 15;
            else
                bit--;
        }
    }

    if (isSigned) {
        int64_t mask = (1ULL << (sigSize - 1));
        if ((result & mask) == mask) // is the highest bit possible for this signal size set?
        {
            /*
             * if so we need to also set every bit higher in the result int too.
             * This leads to the below two lines that are nasty. Here's the theory behind that...
             * If the value is signed and the highest bit is set then it is negative. To create
             * a negative value out of this even though the variable result is 64 bit we have to
             * run 1's all of the way up to bit 63 in result. -1 is all ones for whatever size integer
             * you have. So, it's 64 1's in this case.
             * signedMask is done this way:
             * first you take the signal size and shift 1 up that far. Then subtract one. Lets
             * see that for a 16 bit signal:
             * (1 << 16) - 1 = the first 16 bits set as 1's. So far so good. We then negate the whole
             * thing which flips all bits. Thus signedMask ends up with 1's everwhere that the signal
             * doesn't take up in the 64 bit signed integer result. Then, result has an OR operation on
             * it with the old value and -1 masked so that the the 1 bits from -1 don't overwrite bits from the
             * actual signal. This extends the sign bits out so that the integer result reads as the proper negative
             * value. We dont need to do any of this if the sign bit wasn't set.
             */
            uint64_t signedMask = ~((1ULL << sigSize) - 1);
            result = (-1LL & signedMask) | result;
        }
    }

    return result;
}

void CanSignalCoderPrivate::signalToRaw(const uint32_t id, const CANsignal& sigDesc, const QVariant& sigVal) 
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

void CanSignalCoderPrivate::encodeSignal(const QString& name, const QVariant& val)
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
            if (!_rawCache[id].size()) {
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
