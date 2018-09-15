/***********************************************************************************

This program is provided as is, use it at your own risk

The program was constructed using WebScokets and Qt 5.9.1. Compiling
it with previous versions of qt is not recommended. The following Qt
modules are used: core, serialport and websokets.

Qt can be obtained at: <https://www.qt.io/download>

The program is strongly based on QSerialPort and QWebSockets examples.

Vitor A. M. Jorge

***********************************************************************************/

# Documentation of a Simple Neato WebSocket Control Application

## Goal

This program is a very simple example on how to control a Neato Robot using a web browser. Neato must be connected to a computer through a serial port and this computer should be able to run a simple web server (in this document we use a python3 http server).  

The main purpose of this project is to enable a simple demonstration of robot usage and naive control directed to school grade students.  

## Compiling the Code

Use Qtcretor. If you don't have it, we recommend you to install it --  also vailable using Qt's web installer. Open the .pro project file, run qmake and then build the project. Note that you can, instead, use command line if your Qt distribution is properly installed (qmake, then make). 

Be aware that Qt default system packages are old and some do not install qserialport. On Ubuntu you can install the old package using   

    `$sudo apt-get install libqt5serialport5`  

__However we do not guarantee it will work with different Qt versions than the one tested.__. Install it using the web installer, which seems to be the best way to control Qt versions.  So far, the project was tested using Qt 5.9.1 running Ubuntu 16.04 64 bits. We have noticed that Raspberry Pi 2 running Ubuntu Mate 16.04 presented problems to connect to the Neato serial port -- we are working on a solution.   

## Usage

In order to test the program it is necessary to perform the following steps:  

1. The robot must be connected via USB to the robot  

2. Identify the controller computer IP address and replace the IP address at line 82 of controller.html with it. __Note that the port is 65001__  

4. Launch the WebSocketApplication using:  

    `$ ./WebSocketServer -d -p 65001`  

   which will launch the command receiver at all network interfaces ipa addresses at port 65001. __Note that you can change the port if necessary__ Details about the parameters are presented in the WebSocketApplication section.  __If the serial port presents errors, it means its config is probably incorrect and the robot will not respond__.  


5. Start a web server pointing to the folder containing the file controller.html. If you do not know how to do it, and you are running Ubuntu with python3, start the server as follows  

    `$ cd ~`  
    `$ mkdir public`  
    `$ cd <CODE_DIR>`  
    `$ cp contoller.html ~/public`  
    `$ cd ~/public`  
    `$ sudo python3 -m http.server 1234`  

   which will start a simple web server listening on port 1234, which will contain the controller web page which can be accessed using conventional a browser.  

6. Connect your mobile or computer to the same network where the robot is.  
7. Now you can access the web server from your web browser on your phone. At the menu bar add the following url  

    `<http://<ROBOT_COMPUTER_IP>:1234/controller.html>`  

   A window should open with buttons, to connect and control the robot.  

8. If the WebSocketApplication is running, press the |Connect| button.  
9. Once connected, the robot should respond to user commands.  

   First connect to the robot clicking the *Connect* button Then, if connection is successful you will be able to send commands to the robot pressing any of the control buttons. An example of web interface using Firefox is shown below:  

![alt text](https://raw.githubusercontent.com/vitoramj/SimpleRobotWebRemote/master/ScreenShot.png)  


## WebSocketApplication Manual

The WebSocketApplication is the server listening to the requests via web browser in your mobile. Its usage is simple, with the following options available:  
    -h, --help         Displays this help.  
    -d, --debug        Debug output [default: off].  
    -p, --port <port>  Port for the robot control server [default: 65001].  

The app echoes each and every command sent by the client and checks if the command matches any robot command  


## Limitations  

1. Does not work on default install and Qt 5.5.1 libs of Ubuntu Mate due to failure to detect the Neato port  
2. At this time, the browser client cannot request sensor information.  
3. Currently only movements and playing sound are supported. Even though they are somewhat straightforward to extend.  
4. Connection to other robots is currently under development.  

Enjoy  
