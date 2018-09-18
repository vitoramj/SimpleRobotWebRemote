#include "neatoserialportthread.h"

NeatoSerialPortThread::NeatoSerialPortThread() :
    info(),
    ports(info.availablePorts()),
    port(),
    sleepInterval(30),
    started(false)
{
}

NeatoSerialPortThread::~NeatoSerialPortThread()
{
    shutdownNeato();
}

void NeatoSerialPortThread::run()
{
    // Starting Neato Serial Port
    started = startNeato();
    exec();
}

bool NeatoSerialPortThread::startNeato()
{
    // Initialize Neato Serial Port
    if(ports.size()==0)
    {
        cerr << "No ports available" << endl;
        return false;
    }

    for (int i = 0; i < ports.size(); ++i) {
        cout << "Getting port " << i << " details" << endl;
        cout << "Port Name: " << ports.at(i).portName().toStdString() << endl;
        cout << ports.at(i).manufacturer().toStdString() << "\n" << endl;
    }

    port.setPort(ports.at(0));
    port.open(QIODevice::ReadWrite);
    if(!port.isOpen()){
        cerr << "Failed to open port" << endl;
        return false;
    }
    cout << "Port " << ports.at(0).portName().toStdString() << " opened" << endl;

    if(port.write("PlaySound SoundID 0\n")==-1)
        cerr << "WARNING: Start Sound may have failed" << endl;
    if(port.write("PlaySound SoundID 0\n")==-1)
        cerr << "WARNING: Start Sound may have failed" << endl;
    port.flush();
    cout << "Starting Robot..." << endl;
    if(port.write("TestMode On\n")==-1){
        cerr << "ERROR: Failed to Start TestMode, laser motor will not work." << endl;
        return false;
    }
    port.flush();
    cout << "Configuring TestMode..." << endl;

    if(port.write("SetLDSRotation On\n")==-1) {
        cerr << "ERROR: Laser motor not working." << endl;
        return false;
    }
    port.flush();
    cout << "Starting laser motor..." << endl;


    //QThread::msleep(500);
    // Check if bytes were written
    port.waitForBytesWritten(250);

    cout << "Neato is ready\n\n" << endl;

    return true;
}

void NeatoSerialPortThread::shutdownNeato()
{
    // ShutDown procedure
    if(port.isOpen())
    {
        cout << "\nShutting Down..." << endl;
        if(port.write("PlaySound SoundID 2\n")==-1)
            cerr << "Could not stop laser motor" << endl;
        port.flush();
        cout << "Turning off Laser motor..." << endl;
        if(port.write("SetLDSRotation Off\n")==-1)
            cerr << "Could not stop laser motor" << endl;
        port.flush();
        cout << "Exiting TestMode..." << endl;
        if(port.write("TestMode Off\n")==-1)
            cerr << "Could not exit TestMode" << endl;
        port.flush();

        // Check if bytes were written
        port.waitForBytesWritten(100);

        port.close();
    }
}


void NeatoSerialPortThread::readLaser()
{
    if(port.isOpen())
    {
        // Try to request and read laserdata
        QByteArray stream;
        stream.clear();
        // Read Data procedure
        if(port.write("GetLDSScan\n")==-1) {
            cerr << "Could not request LDS readings" << endl;
            return;
        }
        port.flush();
        while(port.waitForReadyRead(20) || !port.atEnd())
        {
            stream.append(port.readAll());
        }
        cout << stream.toStdString();
    }
    else
        cerr << "Laser reading failure" << endl;

}

void NeatoSerialPortThread::readOdometry()
{
    if(port.isOpen())
    {

        // Try to request and read odometry
        QByteArray stream;
        stream.clear();
        // Read Data procedure
        if(port.write("GetMotors LeftWheel RightWheel\n")==-1) {
            cerr << "Could not request LDS readings" << endl;
            return;
        }
        port.flush();
        while(port.waitForReadyRead(20) || !port.atEnd())
        {
            stream.append(port.readAll());
        }
        std::cout << stream.toStdString();
    }
    else
        cerr << "Odom reading failure" << endl;

}

void NeatoSerialPortThread::readAccel()
{
    if(port.isOpen())
    {
        // Try to request and read anaog sensors
        QByteArray stream;
        stream.clear();
        // Read Data procedure
        if(port.write("GetAnalogSensors\n")==-1) {
            cerr << "Could not request Analog Sensors readings" << endl;
            return;
        }
        port.flush();
        while(port.waitForReadyRead(20) || !port.atEnd())
        {
            stream.append(port.readAll());
        }
        std::cout << stream.toStdString();
    }
    else
        cerr << "Odom Accel failure" << endl;
}


void NeatoSerialPortThread::moveForward()
{
    if(port.isOpen())
    {
        cout << "\nMoving 10cm forward..." << endl;
        if(port.write("SetMotor LWheelDist 100 RWheelDist 100 Speed 100\n")==-1)
            cerr << "Could not move robot" << endl;
        port.flush();
        QThread::msleep(sleepInterval);
    }
    else
        cerr << "Moving Forward failure" << endl;
}

void NeatoSerialPortThread::moveBackward()
{
    if(port.isOpen())
    {
        cout << "\nMoving 10cm back..." << endl;
        if(port.write("SetMotor LWheelDist -100 RWheelDist -100 Speed 100\n")==-1)
            cerr << "Could not move robot" << endl;
        port.flush();
        QThread::msleep(sleepInterval);
    }
    else
        cerr << "Moving back failure" << endl;
}

void NeatoSerialPortThread::turnLeft()
{
    if(port.isOpen())
    {
        cout << "\nMoving to the left..." << endl;
        if(port.write("SetMotor LWheelDist -185 RWheelDist 185 Speed 100\n")==-1)
            cerr << "Could not move robot" << endl;
        port.flush();
        QThread::msleep(sleepInterval);
    }
    else
        cerr << "Moving Left failure" << endl;
}

void NeatoSerialPortThread::turnRight()
{
    if(port.isOpen())
    {
        cout << "\nMoving 10cm back..." << endl;
        if(port.write("SetMotor LWheelDist 185 RWheelDist -185 Speed 100\n")==-1)
            cerr << "Could not move robot" << endl;
        port.flush();
        QThread::msleep(sleepInterval);
    }
    else
        cerr << "Moving Right failure" << endl;
}

void NeatoSerialPortThread::playSound()
{
    if(port.isOpen())
    {
        cout << "\nTalking..." << endl;
        if(port.write("PlaySound SoundID 0\n")==-1)
            cerr << "Could not make robot talk" << endl;
        port.flush();
    }
    else
        cerr << "Talking failure" << endl;
}

void NeatoSerialPortThread::makeMove(int left, int right)
{
    if(port.isOpen())
    {
        QString command("SetMotor LWheelDist ");
        QString num;
        command+=num.setNum(left);
        command.append(" RWheelDist ");
        command+=num.setNum(right);
        command+=QString(" Speed 100\n");

        cout << command.toStdString() << endl;
        if(port.write(command.toLocal8Bit().data(),command.size())==-1)
            cerr << "Could not move robot" << endl;
        port.flush();
        QThread::msleep(sleepInterval);
    }
    else
        cerr << "Free Moving failure" << endl;
}
