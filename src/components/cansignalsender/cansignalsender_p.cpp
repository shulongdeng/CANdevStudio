#include "cansignalsender_p.h"

CanSignalSenderPrivate::CanSignalSenderPrivate(CanSignalSender* q, CanSignalSenderCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalSenderGuiInt>())
    , _tvColumns({ "Id", "Signal name", "Value", "" })
    , q_ptr(q)
{
    initProps();

    _tvModel.setHorizontalHeaderLabels(_tvColumns);
    _ui.initTv(_tvModel);

    _ui.setDockUndockCbk([this] {
        _docked = !_docked;
        emit q_ptr->mainWidgetDockToggled(_ui.mainWidget());
    });

    _ui.setAddCbk([this] { _ui.addRow(_signalNames); });

    _ui.setSendCbk([this](const QString& id, const QString& name, const QVariant& val) {
        uint32_t idNum = id.toUInt(nullptr, 16);
        std::string sigName = fmt::format("0x{:03x}{}_{}", idNum, idNum > 0x7ff ? "x" : "", name.toStdString());
        emit q_ptr->sendSignal(sigName.c_str(), val);
    });
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
