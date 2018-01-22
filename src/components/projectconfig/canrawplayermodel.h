#ifndef CANRAWPLAYERMODEL_H
#define CANRAWPLAYERMODEL_H

#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <QtSerialBus/QCanBusFrame>
#include <canrawplayer.h>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;

class CanRawPlayerModel : public ComponentModel<CanRawPlayer, CanRawPlayerModel> {
    Q_OBJECT

public:
    CanRawPlayerModel();

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    void setInData(std::shared_ptr<NodeData>, PortIndex) override{};
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    QtNodes::NodePainterDelegate* painterDelegate() const override;

    static QColor headerColor1()
    {
        return QColor(144, 187, 62);
    }

    static QColor headerColor2()
    {
        return QColor(84, 84, 84);
    }

public slots:
    void sendFrame(const QCanBusFrame& frame);

private:
    QCanBusFrame _frame;
    std::unique_ptr<NodePainter> _painter;
};

#endif // CANRAWPLAYERMODEL_H
