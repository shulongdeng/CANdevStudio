#ifndef CANSIGNALCODER_H
#define CANSIGNALCODER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>
#include <cantypes.hpp>

class CanSignalCoderPrivate;
class QWidget;
typedef Context<> CanSignalCoderCtx;

class QCanBusFrame;

class CanSignalCoder : public QObject, public ComponentInterface {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CanSignalCoder)

public:
    CanSignalCoder();
    explicit CanSignalCoder(CanSignalCoderCtx&& ctx);
    ~CanSignalCoder();

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
    void sendSignal(const QString& name, const QVariant& value);

public slots:
    void stopSimulation() override;
    void startSimulation() override;
    void canDbUpdated(const CANmessages_t& messages);
    void frameReceived(const QCanBusFrame& frame);

private:
    QScopedPointer<CanSignalCoderPrivate> d_ptr;
};

#endif //CANSIGNALCODER_H
