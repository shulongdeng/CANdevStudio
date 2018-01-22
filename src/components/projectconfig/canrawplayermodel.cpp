#include "canrawplayermodel.h"
#include <datamodeltypes/canrawplayerdata.h>
#include <log.h>

CanRawPlayerModel::CanRawPlayerModel()
    : ComponentModel("CanRawPlayer")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(&_component, &CanRawPlayer::sendFrame, this, &CanRawPlayerModel::sendFrame);
}

QtNodes::NodePainterDelegate* CanRawPlayerModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanRawPlayerModel::nPorts(PortType portType) const
{
    return (PortType::Out == portType) ? 1 : 0;
}

NodeDataType CanRawPlayerModel::dataType(PortType, PortIndex) const
{
    return CanRawPlayerDataOut().type();
}

std::shared_ptr<NodeData> CanRawPlayerModel::outData(PortIndex)
{
    return std::make_shared<CanRawPlayerDataOut>(_frame);
}

void CanRawPlayerModel::sendFrame(const QCanBusFrame& frame)
{
    // TODO: Check if we don't need queue here. If different threads will operate on _frame we may loose data
    _frame = frame;
    emit dataUpdated(0); // Data ready on port 0
}
