#ifndef CANSIGNALSENDERMODEL_H
#define CANSIGNALSENDERMODEL_H

#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <cansignalsender.h>
#include <cantypes.hpp>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;

class CanSignalSenderModel : public ComponentModel<CanSignalSender, CanSignalSenderModel> {
    Q_OBJECT

public:
    CanSignalSenderModel();

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex port) override;
    QtNodes::NodePainterDelegate* painterDelegate() const override;

    static QColor headerColor1()
    {
        return QColor(85, 95, 195);
    }

    static QColor headerColor2()
    {
        return QColor(84, 84, 84);
    }

public slots:
    void sendSignal(const QString& name, const QVariant& val);

signals:
    void canDbUpdated(const CANmessages_t& messages);
    void requestRedraw();

private:
    std::unique_ptr<NodePainter> _painter;
    QString _sigName;
    QVariant _sigVal;
};

#endif // CANSIGNALSENDERMODEL_H
