#include "cansignalcodermodel.h"
#include <datamodeltypes/cansignalcoderdata.h>
#include <log.h>

namespace {

// clang-format off
const std::map<PortType, std::vector<NodeDataType>> portMappings = {
    { PortType::In, 
        {
            {CanSignalCoderDataIn{}.type() },
            {CanSignalCoderSignalIn{}.type() },
            {CanSignalCoderRawIn{}.type() }
        }
    },
    { PortType::Out, 
        {
            {CanSignalCoderSignalOut{}.type()}, 
            {CanSignalCoderRawOut{}.type() }
        }
    }
};
// clang-format on

} // namespace

CanSignalCoderModel::CanSignalCoderModel()
    : ComponentModel("CanSignalCoder")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &CanSignalCoderModel::canDbUpdated, &_component, &CanSignalCoder::canDbUpdated);
    connect(this, &CanSignalCoderModel::frameReceived, &_component, &CanSignalCoder::frameReceived);
}

QtNodes::NodePainterDelegate* CanSignalCoderModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalCoderModel::nPorts(PortType portType) const
{
    return portMappings.at(portType).size();
}

NodeDataType CanSignalCoderModel::dataType(PortType portType, PortIndex ndx) const
{
    if (portMappings.at(portType).size() > static_cast<uint32_t>(ndx)) {
        return portMappings.at(portType)[ndx];
    }
        
    cds_error("No port mapping for ndx: {}", ndx);
    return {};
}

std::shared_ptr<NodeData> CanSignalCoderModel::outData(PortIndex)
{
    // example
    // return std::make_shared<CanDeviceDataOut>(_frame, _direction, _status);

    return {};
}

void CanSignalCoderModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData) {
        if (nodeData->sameType(CanSignalCoderDataIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalCoderDataIn>(nodeData);
            assert(nullptr != d);

            emit canDbUpdated(d->messages());
        } else if (nodeData->sameType(CanSignalCoderRawIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalCoderRawIn>(nodeData);
            assert(nullptr != d);

            emit frameReceived(d->frame());
        } else {
            cds_warn("Incorrect nodeData");
        }
    }
}
