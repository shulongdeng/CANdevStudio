#include "cansignaldata_p.h"

CanSignalDataPrivate::CanSignalDataPrivate(CanSignalData *q, CanSignalDataCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalDataGuiInt>())
    , q_ptr(q)
{
    initProps();
}

void CanSignalDataPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalDataPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalDataPrivate::getSettings()
{
    return { };
}

void CanSignalDataPrivate::setSettings(const QJsonObject& json)
{
    (void)json;
}
