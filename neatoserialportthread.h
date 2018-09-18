#ifndef NEATOSERIALPORTTHREAD_H
#define NEATOSERIALPORTTHREAD_H

#include <QtSerialPort/QtSerialPort>
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#include <QThread>
#include <QTextStream>
#include <QDataStream>
#include <QChar>
#include <QQueue>
#include <QByteArray>
#include <QMutex>

class NeatoSerialPortThread : public QThread
{
   Q_OBJECT

public:
    NeatoSerialPortThread();
    ~NeatoSerialPortThread() override;
    void run() override;

public slots:
    bool startNeato();
    void shutdownNeato();
    void readLaser();
    void readOdometry();
    void readAccel();
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void playSound();
    void makeMove(int left, int right);

protected:
    QSerialPortInfo info;
    QList<QSerialPortInfo> ports;
    QSerialPort port;
    unsigned long sleepInterval;
    bool started;
};

#endif // NEATOSERIALPORTTHREAD_H
