#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "robotcommander.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Neato WebSocket Control Server");

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for the robot control server [default: 65001]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("65001"));
    parser.addHelpOption();
    parser.addOption(dbgOption);
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    RobotCommander *server = new RobotCommander(static_cast<unsigned short>(port), debug);
    QObject::connect(server, &RobotCommander::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
