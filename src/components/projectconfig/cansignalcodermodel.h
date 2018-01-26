#ifndef CANSIGNALCODERMODEL_H
#define CANSIGNALCODERMODEL_H

#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <cansignalcoder.h>
#include <cantypes.hpp>
#include <QCanBusFrame>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;

class CanSignalCoderModel : public ComponentModel<CanSignalCoder, CanSignalCoderModel> {
    Q_OBJECT

public:
    CanSignalCoderModel();

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
    void sendFrame(const QCanBusFrame& frame);

signals:
    void canDbUpdated(const CANmessages_t& messages);
    void frameReceived(const QCanBusFrame& frame);
    void signalReceived(const QString& name, const QVariant& val);

private:
    QCanBusFrame _frame;
    std::unique_ptr<NodePainter> _painter;
};

#endif // CANSIGNALCODERMODEL_H
