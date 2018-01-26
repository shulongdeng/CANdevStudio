#include "cansignalencoder.h"
#include "cansignalencoder_p.h"
#include <confighelpers.h>
#include <log.h>

CanSignalEncoder::CanSignalEncoder()
    : d_ptr(new CanSignalEncoderPrivate(this))
{
}

CanSignalEncoder::CanSignalEncoder(CanSignalEncoderCtx&& ctx)
    : d_ptr(new CanSignalEncoderPrivate(this, std::move(ctx)))
{
}

CanSignalEncoder::~CanSignalEncoder()
{
}

QWidget* CanSignalEncoder::mainWidget()
{
    // Component does not have main widget
    return nullptr;
}

void CanSignalEncoder::setConfig(const QJsonObject& json)
{
    Q_D(CanSignalEncoder);

    d_ptr->setSettings(json);
}

void CanSignalEncoder::setConfig(const QObject& qobject)
{
    Q_D(CanSignalEncoder);

    configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

QJsonObject CanSignalEncoder::getConfig() const
{
    return d_ptr->getSettings();
}

std::shared_ptr<QObject> CanSignalEncoder::getQConfig() const
{
    const Q_D(CanSignalEncoder);

    return configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void CanSignalEncoder::configChanged()
{
}

bool CanSignalEncoder::mainWidgetDocked() const
{
    // Widget does not exist. Return always true
    return true;
}

ComponentInterface::ComponentProperties CanSignalEncoder::getSupportedProperties() const
{
    return d_ptr->getSupportedProperties();
}

void CanSignalEncoder::stopSimulation()
{
    Q_D(CanSignalEncoder);

    d->_simStarted = false;
}

void CanSignalEncoder::startSimulation()
{
    Q_D(CanSignalEncoder);

    d->_simStarted = true;
}

void CanSignalEncoder::canDbUpdated(const CANmessages_t& messages)
{
    d_ptr->_messages = messages;
}

void CanSignalEncoder::signalReceived(const QString& name, const QVariant& val)
{
    cds_info("Signal received: {}, {}", name.toStdString(), val.toString().toStdString());

    d_ptr->encodeSignal(name, val);
}
