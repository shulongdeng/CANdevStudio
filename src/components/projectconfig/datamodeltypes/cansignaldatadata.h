#ifndef CANSIGNALDATADATA_H
#define CANSIGNALDATADATA_H

#include <nodes/NodeDataModel>
#include <cantypes.hpp>

using SignalData_t = std::map<CANmessage, std::vector<CANsignal>>;

class CanSignalDataDataOut : public NodeData {
public:
    CanSignalDataDataOut(){};
    CanSignalDataDataOut(const SignalData_t& messages)
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

#endif // CANSIGNALDATADATA_H
