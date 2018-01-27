#ifndef CANSIGNALVIEWER_H
#define CANSIGNALVIEWER_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <componentinterface.h>
#include <context.h>
#include <memory>

class CanSignalViewerPrivate;
class QWidget;
struct CanSignalViewerGuiInt;
typedef Context<CanSignalViewerGuiInt> CanSignalViewerCtx;

class CanSignalViewer : public QObject, public ComponentInterface {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CanSignalViewer)

public:
    CanSignalViewer();
    explicit CanSignalViewer(CanSignalViewerCtx&& ctx);
    ~CanSignalViewer();

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

public slots:
    void stopSimulation() override;
    void startSimulation() override;

private:
    QScopedPointer<CanSignalViewerPrivate> d_ptr;
};

#endif //CANSIGNALVIEWER_H
