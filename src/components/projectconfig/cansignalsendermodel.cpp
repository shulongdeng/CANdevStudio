#include "cansignalsendermodel.h"
#include <datamodeltypes/cansignalsenderdata.h>
#include <log.h>

namespace {

// clang-format off
const std::map<PortType, std::vector<NodeDataType>> portMappings = {
    { PortType::In, 
        {
            {CanSignalSenderDataIn{}.type() }
        }
    },
    { PortType::Out, 
        {
            {CanSignalSenderSignalOut{}.type()} 
        }
    }
};
// clang-format on

} // namespace

CanSignalSenderModel::CanSignalSenderModel()
    : ComponentModel("CanSignalSender")
    , _painter(std::make_unique<NodePainter>(headerColor1(), headerColor2()))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    _label->setFixedSize(75, 25);
    _label->setAttribute(Qt::WA_TranslucentBackground);

    connect(this, &CanSignalSenderModel::canDbUpdated, &_component, &CanSignalSender::canDbUpdated);
    connect(&_component, &CanSignalSender::sendSignal, this, &CanSignalSenderModel::sendSignal);
}

QtNodes::NodePainterDelegate* CanSignalSenderModel::painterDelegate() const
{
    return _painter.get();
}

unsigned int CanSignalSenderModel::nPorts(PortType portType) const
{
    return portMappings.at(portType).size();
}

NodeDataType CanSignalSenderModel::dataType(PortType portType, PortIndex ndx) const
{
    if (portMappings.at(portType).size() > static_cast<uint32_t>(ndx)) {
        return portMappings.at(portType)[ndx];
    }

    cds_error("No port mapping for ndx: {}", ndx);
    return {};
}

std::shared_ptr<NodeData> CanSignalSenderModel::outData(PortIndex)
{
    return std::make_shared<CanSignalSenderSignalOut>(_sigName, _sigVal, Direction::TX);
}

void CanSignalSenderModel::setInData(std::shared_ptr<NodeData> nodeData, PortIndex)
{
    if (nodeData) {
        if (nodeData->sameType(CanSignalSenderDataIn())) {
            auto d = std::dynamic_pointer_cast<CanSignalSenderDataIn>(nodeData);
            assert(nullptr != d);

            emit canDbUpdated(d->messages());
        } else {
            cds_warn("Incorrect nodeData");
        }
    }
}

void CanSignalSenderModel::sendSignal(const QString& name, const QVariant& val)
{
    _sigName = name;
    _sigVal = val;
    emit dataUpdated(0); // Data ready on port 0
}
