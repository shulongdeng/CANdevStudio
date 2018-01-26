#ifndef CANSIGNALENCODER_H
#define CANSIGNALENCODER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>
#include <cantypes.hpp>

class QCanBusFrame;
class CanSignalEncoderPrivate;
class QWidget;
typedef Context<> CanSignalEncoderCtx;

class CanSignalEncoder : public QObject, public ComponentInterface {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CanSignalEncoder)

public:
    CanSignalEncoder();
    explicit CanSignalEncoder(CanSignalEncoderCtx&& ctx);
    ~CanSignalEncoder();

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
    void sendFrame(const QCanBusFrame& frame);

public slots:
    void stopSimulation() override;
    void startSimulation() override;
    void canDbUpdated(const CANmessages_t& messages);
    void signalReceived(const QString& name, const QVariant& val);

private:
    QScopedPointer<CanSignalEncoderPrivate> d_ptr;
};

#endif //CANSIGNALENCODER_H
