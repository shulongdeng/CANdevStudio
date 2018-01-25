#ifndef CANSIGNALDATADATA_H
#define CANSIGNALDATADATA_H

#include <nodes/NodeDataModel>
#include <cantypes.hpp>

using SignalData_t = std::map<CANmessage, std::vector<CANsignal>>;

class CanSignalDataDataIn : public NodeData {
public:
    CanSignalDataDataIn(){};
    CanSignalDataDataIn(const SignalData_t& messages)
        : _messages(messages)
    {
    }

    NodeDataType type() const override
    {
        return NodeDataType{ "SignalData", "Data" };
    }

    SignalData_t messages() const
    {
        return _messages;
    }

private:
    SignalData_t _messages;
};

using CanSignalDataDataOut = CanSignalDataDataIn;

#endif // CANSIGNALDATADATA_H
