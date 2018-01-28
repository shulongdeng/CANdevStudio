#ifndef CANSIGNALDECODER_H
#define CANSIGNALDECODER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>
#include <cantypes.hpp>

class CanSignalDecoderPrivate;
class QCanBusFrame;
class QWidget;
typedef Context<> CanSignalDecoderCtx;

class CanSignalDecoder : public QObject, public ComponentInterface {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CanSignalDecoder)

public:
    CanSignalDecoder();
    explicit CanSignalDecoder(CanSignalDecoderCtx&& ctx);
    ~CanSignalDecoder();

    QWidget* mainWidget() override;
    void setConfig(const QJsonObject& json) override;
    void setConfig(const QObject& qobject) override;
    QJsonObject getConfig() const override;
    std::shared_ptr<QObject> getQConfig() const override;
    void configChanged() override;
    bool mainWidgetDocked() const override;
    ComponentInterface::ComponentProperties getSupportedProperties() const override;

signals:
    void mainWidgetDockToggled(QWidget* widget) override;
    void signalReceived(const QString& name, const QVariant& value);
    void signalSent(const QString& name, const QVariant& value);

public slots:
    void stopSimulation() override;
    void startSimulation() override;
    void canDbUpdated(const CANmessages_t& messages);
    void frameReceived(const QCanBusFrame& frame);
    void frameSent(const QCanBusFrame& frame);

private:
    QScopedPointer<CanSignalDecoderPrivate> d_ptr;
};

#endif //CANSIGNALDECODER_H
