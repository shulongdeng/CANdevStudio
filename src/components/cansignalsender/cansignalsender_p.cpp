#include "cansignalsender_p.h"

CanSignalSenderPrivate::CanSignalSenderPrivate(CanSignalSender* q, CanSignalSenderCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalSenderGuiInt>())
    , _tvColumns({ "id", "signal name", "value", "" })
    , q_ptr(q)
{
    initProps();
    
    _tvModel.setHorizontalHeaderLabels(_tvColumns);
    _ui.initTv(_tvModel);
}

void CanSignalSenderPrivate::initProps()
{
    for (const auto& p : _supportedProps) {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalSenderPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalSenderPrivate::getSettings()
{
    return {};
}

void CanSignalSenderPrivate::setSettings(const QJsonObject& json)
{
    (void)json;
}

void CanSignalSenderPrivate::canDbUpdated(const CANmessages_t& messages)
{
    _messages = messages;
}

