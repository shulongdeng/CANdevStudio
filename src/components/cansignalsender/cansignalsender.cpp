#include "cansignalsender.h"
#include "cansignalsender_p.h"
#include <confighelpers.h>

CanSignalSender::CanSignalSender()
    : d_ptr(new CanSignalSenderPrivate(this))
{
}

CanSignalSender::CanSignalSender(CanSignalSenderCtx&& ctx)
    : d_ptr(new CanSignalSenderPrivate(this, std::move(ctx)))
{
}

CanSignalSender::~CanSignalSender()
{
}

QWidget* CanSignalSender::mainWidget()
{
    Q_D(CanSignalSender);

    return d->_ui.mainWidget();
}

void CanSignalSender::setConfig(const QJsonObject& json)
{
    Q_D(CanSignalSender);

    d_ptr->setSettings(json);
}

void CanSignalSender::setConfig(const QObject& qobject)
{
    Q_D(CanSignalSender);

    configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

QJsonObject CanSignalSender::getConfig() const
{
    return d_ptr->getSettings();
}

std::shared_ptr<QObject> CanSignalSender::getQConfig() const
{
    const Q_D(CanSignalSender);

    return configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void CanSignalSender::configChanged()
{
}

bool CanSignalSender::mainWidgetDocked() const
{
    return d_ptr->_docked;
}

ComponentInterface::ComponentProperties CanSignalSender::getSupportedProperties() const
{
    return d_ptr->getSupportedProperties();
}

void CanSignalSender::stopSimulation()
{
    Q_D(CanSignalSender);

    d->_simStarted = false;
}

void CanSignalSender::startSimulation()
{
    Q_D(CanSignalSender);

    d->_simStarted = true;
}

void CanSignalSender::canDbUpdated(const CANmessages_t& messages)
{
    d_ptr->_messages = messages;
    d_ptr->_signalNames.clear();

    for(const auto &msg : messages) {
        for(const auto& sig : msg.second) {
            d_ptr->_signalNames[msg.first.id].append(sig.signal_name.c_str());
        }
    }
}


