#include "communicator.h"

Communicator::Communicator()
{
    this->interface = new ClientConnector();
}
