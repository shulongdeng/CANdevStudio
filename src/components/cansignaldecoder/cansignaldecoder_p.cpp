#include "cansignaldecoder_p.h"
#include <log.h>
#include <QCanBusFrame>

CanSignalDecoderPrivate::CanSignalDecoderPrivate(CanSignalDecoder *q, CanSignalDecoderCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanSignalDecoderPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalDecoderPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalDecoderPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalDecoderPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}

void CanSignalDecoderPrivate::decodeFrame(const QCanBusFrame& frame, bool rxDir)
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

            if(rxDir) {
                emit q_ptr->signalReceived(sigName, sigVal);
            } else {
                emit q_ptr->signalSent(sigName, sigVal);
            }

            cds_info("Signal: {}, val: {}", sigName.toStdString(), sigVal.toDouble());
        }

    } else {
        cds_info("Frame '{:x}' does not exist in DB", frame.frameId());
    }
}

// coppied form https://github.com/collin80/SavvyCAN
int64_t CanSignalDecoderPrivate::rawToSignal(
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

const CANmessages_t::value_type* CanSignalDecoderPrivate::findInDb(uint32_t id)
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
