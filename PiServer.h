/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PiServer.h
 * Author: serhiy
 *
 * Created on November 24, 2016, 10:09 AM
 */

#ifndef PISERVER_H
#define PISERVER_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <iostream>
#include "AdafruitMotorHat.h"

#define HAT_ADDRESS 0x60

using namespace std;

class PiServer {
public:
    PiServer();
    PiServer(const PiServer& orig) = default;
    
    virtual ~PiServer();
    bool readMessage();
    bool sendMessage(const char* buff, const int size);
    void setPort(int portNumber);
    int getPort();
    bool portIsOpen();
    string getMessage();
    bool startServer();
    bool startServer(int portNumber);
    bool getIncomingConnection();
    string client_ipstr_;
    void parseMessage(string message);
    AdafruitMotorHat PiCar; // = AdafruitMotorHat(HAT_ADDRESS);
private:
    int socket_;
    int sd_; //socket descriptor
    string buff_;
    bool portIsOpen_;
    int port_;
    bool sendBuffer(const char* buff, const int size);
    bool receiveBuffer(char* buff, const int size);
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> splitMessage(const string &s, char delim);
    int speed_; // -100..0..100
};

#endif /* PISERVER_H */

