#include "cansignalencodermodel.h"
#include <datamodeltypes/cansignalencoderdata.h>
#include <log.h>

namespace {

// clang-format off
const std::map<PortType, std::vector<NodeDataType>> portMappings = {
    { PortType::In, 
        {
            {CanSignalEncoderDataIn{}.type() },
            {CanSignalEncoderSignalIn{}.type() }
        }
    },
    { PortType::Out, 
        {
            {CanSignalEncoderRawOut{}.type() }
        }
    }
};
// clang-format on

} // namespace

CanSignalEncoderModel::CanSignalEncoderModel()
    : ComponentModel("CanSignalEncoder")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &CanSignalEncoderModel::canDbUpdated, &_component, &CanSignalEncoder::canDbUpdated);
    connect(this, &CanSignalEncoderModel::signalReceived, &_component, &CanSignalEncoder::signalReceived);
    connect(&_component, &CanSignalEncoder::sendFrame, this, &CanSignalEncoderModel::sendFrame);
}

QtNodes::NodePainterDelegate* CanSignalEncoderModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalEncoderModel::nPorts(PortType portType) const
{
    return portMappings.at(portType).size();
}

NodeDataType CanSignalEncoderModel::dataType(PortType portType, PortIndex ndx) const
{
    if (portMappings.at(portType).size() > static_cast<uint32_t>(ndx)) {
        return portMappings.at(portType)[ndx];
    }

    cds_error("No port mapping for ndx: { }", ndx);
    return { };
}

std::shared_ptr<NodeData> CanSignalEncoderModel::outData(PortIndex)
{
    cds_debug("Frame requested");
    return std::make_shared<CanSignalEncoderRawOut>(_frame);
}

void CanSignalEncoderModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData) {
        if (nodeData->sameType(CanSignalEncoderDataIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalEncoderDataIn>(nodeData);
            assert(nullptr != d);

            emit canDbUpdated(d->messages());
        } else if (nodeData->sameType(CanSignalEncoderSignalIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalEncoderSignalIn>(nodeData);
            assert(nullptr != d);

            emit signalReceived(d->name(), d->value());
        } else {
            cds_warn("Incorrect nodeData");
        }
    }
}

void CanSignalEncoderModel::sendFrame(const QCanBusFrame& frame) 
{
    cds_debug("Frame received");

    _frame = frame;

    emit dataUpdated(0);
}

