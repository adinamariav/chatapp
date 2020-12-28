#ifndef PROGRAMHANDLER_H
#define PROGRAMHANDLER_H

#include "communicator.h"
#include "mainwindow.h"
#include "clientconnector.h"
#include "chatwindowgenerator.h"
#include "clientinterface.h"
#include <QApplication>
#include <QFile>
#include <QString>

class ProgramHandler : public Communicator
{
public:
    ProgramHandler();
    void startProgram(int argc, char *argv[]);

private:
    int CSocket;


    void createListenerThread();
};

#endif // PROGRAMHANDLER_H
