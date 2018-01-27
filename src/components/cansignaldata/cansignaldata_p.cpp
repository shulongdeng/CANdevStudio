#include "cansignaldata_p.h"
#include <dbcparser.h>
#include <fstream>
#include <log.h>

CanSignalDataPrivate::CanSignalDataPrivate(CanSignalData* q, CanSignalDataCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalDataGuiInt>())
    , _columnsOrder({ "id", "name", "start", "length", "type", "order", "factor", "offset", "min", "max" })
    , _columnsSettings({ "id", "name", "dlc", "ecu", "cycle", "initial value" })
    , q_ptr(q)
{
    initProps();

    _tvModelSettings.setHorizontalHeaderLabels(_columnsSettings);
    _tvModel.setHorizontalHeaderLabels(_columnsOrder);
    _tvModelFilter.setSourceModel(&_tvModel);
    _ui.initTableView(_tvModelFilter);
    _ui.initSearch(_tvModelFilter);
    
    _tvModelSettingsFilter.setSourceModel(&_tvModelSettings);
    _ui.initSearch(_tvModelSettingsFilter);

    _ui.setSettingsCbk([this] {
        _settings = !_settings;

        if(_settings) {
            _ui.initSettings(_tvModelSettingsFilter);
        } else {
            _ui.initTableView(_tvModelFilter);
        }
    });

    _ui.setDockUndockCbk([this] {
        _docked = !_docked;
        emit q_ptr->mainWidgetDockToggled(_ui.mainWidget());
    });
}

void CanSignalDataPrivate::initProps()
{
    for (const auto& p : _supportedProps) {
        _props[p.first];
    }
}

ComponentInterface::ComponentProperties CanSignalDataPrivate::getSupportedProperties() const
{
    return _supportedProps;
}

QJsonObject CanSignalDataPrivate::getSettings()
{
    QJsonObject json;

    for (const auto& p : _props) {
        json[p.first] = QJsonValue::fromVariant(p.second);
    }

    return json;
}

void CanSignalDataPrivate::setSettings(const QJsonObject& json)
{
    for (const auto& p : _supportedProps) {
        if (json.contains(p.first))
            _props[p.first] = json[p.first].toVariant();
    }
}

std::string CanSignalDataPrivate::loadFile(const std::string& filename)
{
    const std::string path = filename;

    std::fstream file{ path.c_str() };

    if (!file.good()) {
        cds_error("File {} does not exists", filename);
    }

    std::string buff;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(buff));

    file.close();
    return buff;
}

void CanSignalDataPrivate::loadDbc(const std::string& filename)
{
    CANdb::DBCParser parser;
    bool success = parser.parse(loadFile(filename));

    if (!success) {
        cds_error("Failed to load CAN DB from '{}' file", filename);
        return;
    }

    _canDb = parser.getDb().messages;

    cds_info("CAN DB load successful. {} records found", _canDb.size());

    emit q_ptr->canDbUpdated(_canDb);

    _tvModel.removeRows(0, _tvModel.rowCount());

    for(auto &message : _canDb) {
        QList<QStandardItem*> settingsList;
        uint32_t id = message.first.id;
        QString frameID = QString("0x" + QString::number(id, 16));

        settingsList.append(new QStandardItem(frameID));
        settingsList.append(new QStandardItem(message.first.name.c_str()));
        settingsList.append(new QStandardItem(QString::number(message.first.dlc)));
        settingsList.append(new QStandardItem(message.first.ecu.c_str()));

        for(auto &i : settingsList) {
            i->setEditable(false);
        }

        _tvModelSettings.appendRow(settingsList);

        for(auto &signal : message.second) {
            QList<QStandardItem*> list;

            list.append(new QStandardItem(frameID));
            list.append(new QStandardItem(signal.signal_name.c_str()));
            list.append(new QStandardItem(QString::number(signal.startBit)));
            list.append(new QStandardItem(QString::number(signal.signalSize)));
            list.append(new QStandardItem(signal.valueSigned ? "signed" : "unsigned"));
            list.append(new QStandardItem(QString::number(signal.byteOrder)));
            list.append(new QStandardItem(QString::number(signal.factor)));
            list.append(new QStandardItem(QString::number(signal.offset)));
            list.append(new QStandardItem(QString::number(signal.min)));
            list.append(new QStandardItem(QString::number(signal.max)));

            for(auto &i : list) {
                i->setEditable(false);
            }

            _tvModel.appendRow(list);
        }
    }
}
