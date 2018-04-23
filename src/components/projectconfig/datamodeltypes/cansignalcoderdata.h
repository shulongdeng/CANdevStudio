#ifndef CANSIGNALCODERDATA_H
#define CANSIGNALCODERDATA_H

#include "candevicedata.h"
#include "cansignaldatadata.h"

using CanSignalCoderRawOut = CanDeviceDataIn;
using CanSignalCoderRawIn = CanDeviceDataOut;
using CanSignalCoderDataIn = CanSignalDataDataOut;

class CanSignalCoderSignalOut : public NodeData {
public:
    CanSignalCoderSignalOut(){};
    CanSignalCoderSignalOut(const QString& sigName, const QVariant& value)
        : _sigName(sigName)
        , _value(value)
    {
    }

    NodeDataType type() const override
    {
        return NodeDataType{ "SignalCoder", "SIG"};
    }

    QVariant value() const
    {
        return _value;
    }

    QString name() const 
    {
        return _sigName;
    }

private:
    QString _sigName;
    QVariant _value;
};

using CanSignalCoderSignalIn = CanSignalCoderSignalOut;

#endif // CANSIGNALCODERDATA_H
