#include "cansignaldata_p.h"
#include <dbcparser.h>
#include <fstream>
#include <log.h>

CanSignalDataPrivate::CanSignalDataPrivate(CanSignalData* q, CanSignalDataCtx&& ctx)
    : _ctx(std::move(ctx))
    , _ui(_ctx.get<CanSignalDataGuiInt>())
    , q_ptr(q)
{
    initProps();
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

    const auto& db = parser.getDb();

    cds_info("CAN DB load successful. {} records found", db.messages.size());
}
