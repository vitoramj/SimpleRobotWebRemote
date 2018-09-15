#include "robotcommander.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QNetworkInterface>

QT_USE_NAMESPACE

RobotCommander::RobotCommander(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Robot Server"), QWebSocketServer::NonSecureMode, this)),
    m_debug(debug),
    robot(),
    timer()
{
    // Connects server and robot and starts the robot
    robot.start();
    connect(this,SIGNAL(turnLeftCommand()), &robot,SLOT(turnLeft()));
    connect(this,SIGNAL(moveBackwardCommand()), &robot,SLOT(moveBackward()));
    connect(this,SIGNAL(moveForwardCommand()), &robot,SLOT(moveForward()));
    connect(this,SIGNAL(turnRightCommand()), &robot,SLOT(turnRight()));
    connect(this,SIGNAL(playSoundCommand()), &robot,SLOT(playSound()));


    // Get IP and display it on screen
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug() << "The server is running on \nIP: " << ipAddress;

    // Makes WebSocket Listen to all host addresses
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "listening on port" << port;

        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &RobotCommander::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &RobotCommander::closed);
    }

    // Testing init
//    timer.setInterval(1000);
//    timer.setSingleShot(false);
//    connect(&timer,SIGNAL(timeout()),this,SLOT(sendKeepAlive()),Qt::QueuedConnection);
//    timer.start();
}

RobotCommander::~RobotCommander()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void RobotCommander::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &RobotCommander::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &RobotCommander::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &RobotCommander::socketDisconnected);

    m_clients << pSocket;
}

void RobotCommander::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Message received:" << message;
    if (pClient) {
        pClient->sendTextMessage(message);
    }

    if(message == QString("Left"))
        emit turnLeftCommand();
    if(message == QString("Right"))
        emit turnRightCommand();
    if(message == QString("Backward"))
        emit moveBackwardCommand();
    if(message == QString("Forward"))
        emit moveForwardCommand();
    if(message == QString("Talk"))
        emit playSoundCommand();
}

// Gets the incoming info and sends it back
void RobotCommander::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}
void RobotCommander::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

// Test funcion to send messages to the client according to a timer
void RobotCommander::sendKeepAlive()
{
    // A simple message actively sent by the server
    QByteArray message("Server is live");
    for(int i=0; i<m_clients.size(); ++i)
        m_clients.at(i)->sendTextMessage(message);
}
