#ifndef CANSIGNALDATAGUIINT_H
#define CANSIGNALDATAGUIINT_H

#include <Qt>
#include <functional>

class QWidget;

struct CanSignalDataGuiInt {
    virtual ~CanSignalDataGuiInt()
    {
    }

    virtual QWidget* mainWidget() = 0;
};

#endif // CANSIGNALDATAGUIINT_H
