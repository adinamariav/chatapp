#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include "clientconnector.h"


class Communicator
{
public:
    Communicator();

protected:
    ClientInterface* interface;
};

#endif // COMMUNICATOR_H
