#ifndef ROBOTCOMMANDER_H
#define ROBOTCOMMANDER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include "neatoserialportthread.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class RobotCommander : public QObject
{
    Q_OBJECT
public:
    explicit RobotCommander(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~RobotCommander();
    int left;
    int right;

Q_SIGNALS:
    void closed();

    // Robot control signals
    void startRobotCommand();
    void turnLeftCommand();
    void moveBackwardCommand();
    void moveForwardCommand();
    void turnRightCommand();
    void playSoundCommand();
    void sendMoveCommand(const int left, const int right);
    void shutdownRobotCommand();


private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

    // testing
    //void sendKeepAlive();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
    NeatoSerialPortThread robot;

    // Testing timer to send variables
    QTimer timer;
};

#endif // ROBOTCOMMANDER_H
