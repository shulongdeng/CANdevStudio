#ifndef CANSIGNALDECODERMODEL_H
#define CANSIGNALDECODERMODEL_H

#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <cansignaldecoder.h>
#include <cantypes.hpp>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;
class QCanBusFrame;

class CanSignalDecoderModel : public ComponentModel<CanSignalDecoder, CanSignalDecoderModel> {
    Q_OBJECT

public:
    CanSignalDecoderModel();

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
    void signalReceived(const QString& name, const QVariant& value);
    void signalSent(const QString& name, const QVariant& value);

signals:
    void canDbUpdated(const CANmessages_t& messages);
    void frameReceived(const QCanBusFrame& frame);
    void frameSent(const QCanBusFrame& frame);

private:
    QString _name;
    QVariant _value;
    Direction _dir;
    std::unique_ptr<NodePainter> _painter;
};

#endif // CANSIGNALDECODERMODEL_H
