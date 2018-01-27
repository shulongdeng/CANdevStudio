#include "cansignalencoder.h"
#include "cansignalencoder_p.h"
#include <confighelpers.h>
#include <log.h>
#include <QCanBusFrame>

CanSignalEncoder::CanSignalEncoder()
    : d_ptr(new CanSignalEncoderPrivate(this))
{
}

CanSignalEncoder::CanSignalEncoder(CanSignalEncoderCtx&& ctx)
    : d_ptr(new CanSignalEncoderPrivate(this, std::move(ctx)))
{
}

CanSignalEncoder::~CanSignalEncoder() {}

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

void CanSignalEncoder::configChanged() {}

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

    for(auto &timer : d_ptr->_cycleTimers) {
        timer->stop();
    }
}

void CanSignalEncoder::startSimulation()
{
    Q_D(CanSignalEncoder);

    d->_simStarted = true;

    for(auto &timer : d_ptr->_cycleTimers) {
        timer->start();
    }
}

void CanSignalEncoder::canDbUpdated(const CANmessages_t& messages)
{
    d_ptr->_messages = messages;

    d_ptr->_cycleTimers.clear();

    for (const auto& msg : messages) {
        if (msg.first.initValue.length()) {
            d_ptr->_rawCache[msg.first.id] = QByteArray::fromHex(msg.first.initValue.c_str());
        }

        if (msg.first.updateCycle) {
            QTimer *timer = new QTimer;
            timer->setInterval(msg.first.updateCycle);
            connect(timer, &QTimer::timeout, [ this, msg ] {
                QCanBusFrame frame;
                frame.setFrameId(msg.first.id);
                if (d_ptr->_rawCache[msg.first.id].size() < (int)msg.first.dlc) {
                    cds_info("Setting up new cache for {:03x} msg", msg.first.id);
                    // Set chache for the first time
                    d_ptr->_rawCache[msg.first.id].fill(0, msg.first.dlc);
                }

                frame.setPayload(d_ptr->_rawCache[msg.first.id]);
                emit sendFrame(frame);
            });

            d_ptr->_cycleTimers.push_back(std::make_unique<QTimer>());
            d_ptr->_cycleTimers.back().reset(timer);
        }

        if (msg.first.updateCycle || msg.first.initValue.length()) {
            cds_debug("Settings for msg '{:03x}', cycle: {}, initValue: {}", msg.first.id, msg.first.updateCycle,
                msg.first.initValue);
        }
    }
}

void CanSignalEncoder::signalReceived(const QString& name, const QVariant& val)
{
    if (d_ptr->_simStarted) {
        cds_info("Signal received: {}, {}", name.toStdString(), val.toString().toStdString());

        d_ptr->encodeSignal(name, val);
    }
}
