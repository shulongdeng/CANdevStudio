#include "cansignalviewer_p.h"
#include <log.h>

CanSignalViewerPrivate::CanSignalViewerPrivate(CanSignalViewer *q, CanSignalViewerCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalViewerGuiInt>())
    , q_ptr(q)
{
    initProps();

    _tvModel.setHorizontalHeaderLabels(_columns);
    _ui.initTableView(_tvModel);

    _ui.setClearCbk(std::bind(&CanSignalViewerPrivate::clear, this));

    _ui.setDockUndockCbk([this] {
        _docked = !_docked;
        emit q_ptr->mainWidgetDockToggled(_ui.mainWidget());
    });
}

void CanSignalViewerPrivate::initProps()
{
    for (const auto& p: _supportedProps)
    {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalViewerPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalViewerPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalViewerPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}

void CanSignalViewerPrivate::addSignal(const QString& id, const QString& name, const QString& value, const QString& dir)
{
    QList<QStandardItem*> list;
    QString time = QString::number((_timer.elapsed() / 1000.0), 'f', 2);

    list.append(new QStandardItem(QString::number(_rowId++)));
    list.append(new QStandardItem(time));
    list.append(new QStandardItem(id));
    list.append(new QStandardItem(dir));
    list.append(new QStandardItem(name));
    list.append(new QStandardItem(value));

    for(auto &i : list) {
        i->setEditable(false);
    }

    _tvModel.appendRow(list);

    _ui.newRowAdded();
}

void CanSignalViewerPrivate::clear()
{
    _tvModel.removeRows(0, _tvModel.rowCount());
}
