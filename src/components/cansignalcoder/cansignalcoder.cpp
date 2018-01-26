#include "cansignalcoder.h"
#include "cansignalcoder_p.h"
#include <confighelpers.h>
#include <log.h>
#include <QCanBusFrame>

CanSignalCoder::CanSignalCoder()
    : d_ptr(new CanSignalCoderPrivate(this))
{
}

CanSignalCoder::CanSignalCoder(CanSignalCoderCtx&& ctx)
    : d_ptr(new CanSignalCoderPrivate(this, std::move(ctx)))
{
}

CanSignalCoder::~CanSignalCoder()
{
}

QWidget* CanSignalCoder::mainWidget()
{
    // Component does not have main widget
    return nullptr;
}

void CanSignalCoder::setConfig(const QJsonObject& json)
{
    Q_D(CanSignalCoder);

    d_ptr->setSettings(json);
}

void CanSignalCoder::setConfig(const QObject& qobject)
{
    Q_D(CanSignalCoder);

    configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

QJsonObject CanSignalCoder::getConfig() const
{
    return d_ptr->getSettings();
}

std::shared_ptr<QObject> CanSignalCoder::getQConfig() const
{
    const Q_D(CanSignalCoder);

    return configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void CanSignalCoder::configChanged()
{
}

bool CanSignalCoder::mainWidgetDocked() const
{
    // Widget does not exist. Return always true
    return true;
}

ComponentInterface::ComponentProperties CanSignalCoder::getSupportedProperties() const
{
    return d_ptr->getSupportedProperties();
}

void CanSignalCoder::stopSimulation()
{
    Q_D(CanSignalCoder);

    d->_simStarted = false;
}

void CanSignalCoder::startSimulation()
{
    Q_D(CanSignalCoder);

    d->_simStarted = true;
}

void CanSignalCoder::canDbUpdated(const CANmessages_t& messages)
{
    d_ptr->_messages = messages;
}

void CanSignalCoder::frameReceived(const QCanBusFrame& frame)
{
    d_ptr->decodeFrame(frame);    
}

void CanSignalCoder::signalReceived(const QString& name, const QVariant& val)
{
    cds_info("Signal received: {}, {}", name.toStdString(), val.toString().toStdString());

    d_ptr->encodeSignal(name, val);
}

