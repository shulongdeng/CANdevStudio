#include "canrawplayer_p.h"
#include <QFile>
#include <QFileInfo>
#include <log.h>

CanRawPlayerPrivate::CanRawPlayerPrivate(CanRawPlayer* q, CanRawPlayerCtx&& ctx)
    : _ctx(std::move(ctx))
    , q_ptr(q)
{
    initProps();
}

void CanRawPlayerPrivate::initProps()
{
    for (const auto& p : _supportedProps) {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanRawPlayerPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanRawPlayerPrivate::getSettings()
{
    return {};
}

void CanRawPlayerPrivate::setSettings(const QJsonObject& json)
{
    (void)json;
}

void CanRawPlayerPrivate::loadTraceFile(const QString& filename)
{
    QFileInfo fileNfo(filename);

    if (fileNfo.exists() && fileNfo.isFile()) {
    } else {
        cds_warn("File: '{}' does not exist", filename.toStdString());
    }
}
