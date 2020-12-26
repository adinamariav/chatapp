#include "programhandler.h"

ProgramHandler::ProgramHandler()
{

}

void ProgramHandler::startProgram(int argc, char *argv[])
{
    QApplication a(argc, argv);



   // this->interface->initiateConnection();
    this->CSocket = interface->getSocket();

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
