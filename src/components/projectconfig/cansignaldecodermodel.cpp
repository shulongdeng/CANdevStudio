#include "cansignaldecodermodel.h"
#include <datamodeltypes/cansignaldecoderdata.h>
#include <log.h>

namespace {

// clang-format off
const std::map<PortType, std::vector<NodeDataType>> portMappings = {
    { PortType::In, 
        {
            {CanSignalDecoderDataIn{}.type() },
            {CanSignalDecoderRawIn{}.type() },
        }
    },
    { PortType::Out, 
        {
            {CanSignalDecoderSignalOut{}.type()}, 
        }
    }
};
// clang-format on

} // namespace

CanSignalDecoderModel::CanSignalDecoderModel()
    : ComponentModel("CanSignalDecoder")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &CanSignalDecoderModel::canDbUpdated, &_component, &CanSignalDecoder::canDbUpdated);
    connect(this, &CanSignalDecoderModel::frameReceived, &_component, &CanSignalDecoder::frameReceived);
}

QtNodes::NodePainterDelegate* CanSignalDecoderModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalDecoderModel::nPorts(PortType portType) const
{
    return portMappings.at(portType).size();
}

NodeDataType CanSignalDecoderModel::dataType(PortType portType, PortIndex ndx) const
{
    if (portMappings.at(portType).size() > static_cast<uint32_t>(ndx)) {
        return portMappings.at(portType)[ndx];
    }

    cds_error("No port mapping for ndx: { }", ndx);
    return { };
}

std::shared_ptr<NodeData> CanSignalDecoderModel::outData(PortIndex)
{
    // example
    // return std::make_shared<CanDeviceDataOut>(_frame, _direction, _status);

    return { };
}

void CanSignalDecoderModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData) {
        if (nodeData->sameType(CanSignalDecoderDataIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalCoderDataIn>(nodeData);
            assert(nullptr != d);

            emit canDbUpdated(d->messages());
        } else if (nodeData->sameType(CanSignalDecoderRawIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalCoderRawIn>(nodeData);
            assert(nullptr != d);

            // We are interested in RX frames only!
            if(d->direction() == Direction::RX) {
                emit frameReceived(d->frame());
            }
        } else {
            cds_warn("Incorrect nodeData");
        }
    }
}
