#include "cansignalviewer.h"
#include "cansignalviewer_p.h"
#include <confighelpers.h>
#include <log.h>

CanSignalViewer::CanSignalViewer()
    : d_ptr(new CanSignalViewerPrivate(this))
{
}

CanSignalViewer::CanSignalViewer(CanSignalViewerCtx&& ctx)
    : d_ptr(new CanSignalViewerPrivate(this, std::move(ctx)))
{
}

CanSignalViewer::~CanSignalViewer()
{
}

QWidget* CanSignalViewer::mainWidget()
{
    Q_D(CanSignalViewer);

    return d->_ui.mainWidget();
}

void CanSignalViewer::setConfig(const QJsonObject& json)
{
    Q_D(CanSignalViewer);

    d_ptr->setSettings(json);
}

void CanSignalViewer::setConfig(const QObject& qobject)
{
    Q_D(CanSignalViewer);

    configHelpers::setQConfig(qobject, getSupportedProperties(), d->_props);
}

QJsonObject CanSignalViewer::getConfig() const
{
    return d_ptr->getSettings();
}

std::shared_ptr<QObject> CanSignalViewer::getQConfig() const
{
    const Q_D(CanSignalViewer);

    return configHelpers::getQConfig(getSupportedProperties(), d->_props);
}

void CanSignalViewer::configChanged()
{
}

bool CanSignalViewer::mainWidgetDocked() const
{
    return d_ptr->_docked;
}

ComponentInterface::ComponentProperties CanSignalViewer::getSupportedProperties() const
{
    return d_ptr->getSupportedProperties();
}

void CanSignalViewer::stopSimulation()
{
    Q_D(CanSignalViewer);

    d->_simStarted = false;
}

void CanSignalViewer::startSimulation()
{
    Q_D(CanSignalViewer);

    d->_simStarted = true;
}
