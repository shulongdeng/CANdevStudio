#ifndef CANSIGNALENCODERDATA_H
#define CANSIGNALENCODERDATA_H

#include "candevicedata.h"
#include "cansignaldatadata.h"

using CanSignalEncoderRawOut = CanDeviceDataIn;
using CanSignalEncoderDataIn = CanSignalDataDataOut;

class CanSignalEncoderSignalIn : public NodeData {
public:
    CanSignalEncoderSignalIn(){};
    CanSignalEncoderSignalIn(const QString& sigName, const QVariant& value, const Direction& dir)
        : _sigName(sigName)
        , _value(value)
        , _dir(dir)
    {
    }

    NodeDataType type() const override
    {
        return NodeDataType{ "SignalEncoder", "SIG"};
    }

    QVariant value() const
    {
        return _value;
    }

    QString name() const 
    {
        return _sigName;
    }

    Direction direction() const
    {
        return _dir;
    }

private:
    QString _sigName;
    QVariant _value;
    Direction _dir;
};

#endif // CANSIGNALENCODERDATA_H
