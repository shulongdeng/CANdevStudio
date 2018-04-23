#ifndef CANSIGNALSENDER_H
#define CANSIGNALSENDER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>
#include <cantypes.hpp>

class CanSignalSenderPrivate;
class QWidget;
struct CanSignalSenderGuiInt;
typedef Context<CanSignalSenderGuiInt> CanSignalSenderCtx;

class CanSignalSender : public QObject, public ComponentInterface {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CanSignalSender)

public:
    CanSignalSender();
    explicit CanSignalSender(CanSignalSenderCtx&& ctx);
    ~CanSignalSender();

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
    void sendSignal(const QString& name, const QVariant& val);

public slots:
    void stopSimulation() override;
    void startSimulation() override;
    void canDbUpdated(const CANmessages_t& messages);

private:
    QScopedPointer<CanSignalSenderPrivate> d_ptr;
};

#endif //CANSIGNALSENDER_H
