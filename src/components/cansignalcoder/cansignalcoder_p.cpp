#include "cansignalcoder_p.h"

CanSignalCoderPrivate::CanSignalCoderPrivate(CanSignalCoder *q, CanSignalCoderCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanSignalCoderPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalCoderPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalCoderPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalCoderPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}
