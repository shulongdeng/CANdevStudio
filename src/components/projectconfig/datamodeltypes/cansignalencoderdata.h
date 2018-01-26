#ifndef CANSIGNALENCODERDATA_H
#define CANSIGNALENCODERDATA_H

#include "candevicedata.h"
#include "cansignaldatadata.h"

using CanSignalEncoderRawOut = CanDeviceDataIn;
using CanSignalEncoderDataIn = CanSignalDataDataOut;

class CanSignalEncoderSignalIn : public NodeData {
public:
    CanSignalEncoderSignalIn(){};
    CanSignalEncoderSignalIn(const QString& sigName, const QVariant& value)
        : _sigName(sigName)
        , _value(value)
    {
    }

    NodeDataType type() const override
    {
        return NodeDataType{ "SignalEncoder", "Sig"};
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

#endif // CANSIGNALENCODERDATA_H
