#include "canrawplayer_p.h"

CanRawPlayerPrivate::CanRawPlayerPrivate(CanRawPlayer *q, CanRawPlayerCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanRawPlayerPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanRawPlayerPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanRawPlayerPrivate::getSettings()
{
    return { };
}

void CanRawPlayerPrivate::setSettings(const QJsonObject& json)
{
    (void)json;
}
