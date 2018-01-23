#ifndef CANRAWPLAYER_P_H
#define CANRAWPLAYER_P_H

#include <QtCore/QObject>
#include <memory>
#include "canrawplayer.h"

class CanRawPlayer;

class CanRawPlayerPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CanRawPlayer)

public:
    CanRawPlayerPrivate(CanRawPlayer* q, CanRawPlayerCtx&& ctx = CanRawPlayerCtx());
    ComponentInterface::ComponentProperties getSupportedProperties() const;
    QJsonObject getSettings();
    void setSettings(const QJsonObject& json);
    void loadTraceFile(const QString& filename);

private:
    void initProps();

public:
    bool _simStarted{ false };
    CanRawPlayerCtx _ctx;
    std::map<QString, QVariant> _props;

private:
    CanRawPlayer* q_ptr;
    const QString _nameProperty = "name";
    const QString _fileProperty = "file";
    ComponentInterface::ComponentProperties _supportedProps = {
            {_nameProperty,   {QVariant::String, true}},
            {_fileProperty,   {QVariant::String, true}}
    };
};

#endif // CANRAWPLAYER_P_H
