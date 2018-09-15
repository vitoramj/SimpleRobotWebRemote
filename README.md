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


This program is a very simple example on how to control a Neato Robot using a web browser.
Neato must be connected to a computer through a serial port and running a simple web server.  

The main purpose is for demonstration to school grade students.  

## Compiling the Code

Use qtcretor. If you don't have it, we recommend you to install it. Then run' run qmake and then build all.
Otherwise you can use command line  


## Usage

In order to test the program it is necessary to perform the following steps:  

1. The robot must be connected via USB to the robot  

2. Identify the controller computer IP address and replace the at line 82 of controller.html with it. __Note that the port is 65001__  

4. Launch the WebSocketApplication using:  

    `$ ./WebSocketServer -d -p 65001`  

   which will launch the command receiver at all network interfaces ipa addresses at port 65001. __Note that you can change it if necessary. If the serial port presents errors, it means  the serialport config is probably incorrect and the robot will not respond__.  

5. Start a web server pointing to the folder containing the file controller.html. If you do not know how to to do it.
   Start the server as follows  

    `$ cd ~`  
    `$ mkdir public`  
    `$ cd <FILE_LOCATION>`  
    `$ cp contoller.html ~/public`  
    `$ sudo python3 -m http.server 1234`  

   which will start a simple web server listening on port 1234, which will contain the controller web page for access through a browser.  

6. Connect your mobile or computer to the same network where the robot is.  
7. Now you can access the web server from your web browser. At the menu bar add the following url  

    `<http://<CONTROLLER_COMPUTER_IP>:1234/controller.html>`  

   A window should open with buttons, to connect and control the robot.  

8. If the WebSocketpplication is running, press the |Connect| button.  
9. Once connected, the robot should respond to user commands.  


## WebSocketApplication Guide

The application usage is simple, with the following options available:  
    -h, --help         Displays this help.  
    -d, --debug        Debug output [default: off].  
    -p, --port <port>  Port for the robot control server [default: 65001].  

The app echoes each and every command sent by the client and checks if the command matches any robot command  

First connect to the robot clicking the *Connect* button  
Then, if connection is successful you will be able to send commands to the robot pressing any of the control buttons  

![alt text](https://raw.githubusercontent.com/vitoramj/SimpleRobotWebRemote/master/ScreenShot.png)  


## Limitations  

1. At this time the browser client cannot receive sensor information.  
2. Connection to other robots is currently under development.  

Enjoy  
