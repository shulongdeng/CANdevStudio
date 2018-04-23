#include "cansignaldatamodel.h"
#include <datamodeltypes/cansignaldatadata.h>
#include <log.h>

CanSignalDataModel::CanSignalDataModel()
    : ComponentModel("CanSignalData")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(&_component, &CanSignalData::canDbUpdated, this, &CanSignalDataModel::canDbUpdated);
}

QtNodes::NodePainterDelegate* CanSignalDataModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalDataModel::nPorts(PortType portType) const
{
    return (PortType::Out == portType) ? 3 : 0;
}

NodeDataType CanSignalDataModel::dataType(PortType, PortIndex) const
{
    return CanSignalDataDataOut{}.type();
}

std::shared_ptr<NodeData> CanSignalDataModel::outData(PortIndex)
{
    return std::make_shared<CanSignalDataDataOut>(_messages);
}

void CanSignalDataModel::canDbUpdated(const SignalData_t& messages)
{
    const auto portCnt = nPorts(PortType::Out);

    _messages = messages;

    for(uint32_t i = 0; i < portCnt; ++i) {
        emit dataUpdated(i); // Data ready on port 0
    } 
}

