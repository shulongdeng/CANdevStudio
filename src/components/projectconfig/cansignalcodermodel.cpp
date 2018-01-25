#include "cansignalcodermodel.h"
#include <datamodeltypes/cansignalcoderdata.h>
#include <log.h>

CanSignalCoderModel::CanSignalCoderModel()
    : ComponentModel("CanSignalCoder")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);
}

QtNodes::NodePainterDelegate* CanSignalCoderModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalCoderModel::nPorts(PortType portType) const
{
    // example
    // assert((PortType::In == portType) || (PortType::Out == portType) || (PortType::None == portType)); // range check
    // return (PortType::None != portType) ? 1 : 0;
    (void) portType;

    return { };
}

NodeDataType CanSignalCoderModel::dataType(PortType portType, PortIndex) const
{
    // example
    // assert((PortType::In == portType) || (PortType::Out == portType)); // allowed input
    // return (PortType::Out == portType) ? CanDeviceDataOut{}.type() : CanDeviceDataIn{}.type();
    (void) portType;

    return { };
}

std::shared_ptr<NodeData> CanSignalCoderModel::outData(PortIndex)
{
    // example
    // return std::make_shared<CanDeviceDataOut>(_frame, _direction, _status);

    return { };
}

void CanSignalCoderModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    // example
    // if (nodeData) {
    //     auto d = std::dynamic_pointer_cast<CanDeviceDataIn>(nodeData);
    //     assert(nullptr != d);
    //     emit sendFrame(d->frame());
    // } else {
    //     cds_warn("Incorrect nodeData");
    // }
    (void) nodeData;
}
