#ifndef CANSIGNALDATAMODEL_H
#define CANSIGNALDATAMODEL_H

#include "componentmodel.h"
#include "nodepainter.h"
#include <QtCore/QObject>
#include <cansignaldata.h>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

enum class Direction;

class CanSignalDataModel : public ComponentModel<CanSignalData, CanSignalDataModel> {
    Q_OBJECT

public:
    CanSignalDataModel();

    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, PortIndex) override {}
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
    void canDbUpdated(const SignalData_t& messages);

signals:

private:
    std::unique_ptr<NodePainter> _painter;
    SignalData_t _messages;
};

#endif // CANSIGNALDATAMODEL_H
