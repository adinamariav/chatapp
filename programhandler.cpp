#include "programhandler.h"
#include "chatlistener.h"

ProgramHandler::ProgramHandler()
{

}

void ProgramHandler::startProgram(int argc, char *argv[])
{
    QApplication a(argc, argv);

    this->interface->initiateConnection();
    this->CSocket = interface->getSocket();
    createListenerThread();

    MainWindow* w = new MainWindow(this->CSocket);

    QFile File(":/new/theme/Combinear.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    a.setStyleSheet(StyleSheet);

    w->show();

   /* for(int i = 0; i < 3; i++)
    {
        auto window = ChatWindowGenerator::getNewChatWindow("cineva");
        window->show();
    }*/

    a.exec();
    interface->endConnection();
}

void ProgramHandler::createListenerThread()
{
    QThread* thread = new QThread;
    auto listener = ChatListener::getInstance(this->CSocket);
    listener->moveToThread(thread);

    QObject::connect(thread, SIGNAL(started()), listener, SLOT(process()));
    QObject::connect(listener, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(listener, SIGNAL(finished()), listener, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
