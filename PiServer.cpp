/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PiServer.cpp
 * Author: serhiy
 * 
 * Created on November 24, 2016, 10:09 AM
 */

#include "PiServer.h"

PiServer::PiServer() {
    port_ = -1;
    portIsOpen_ = false;
}

PiServer::PiServer(const PiServer& orig) {
}

PiServer::~PiServer() {
}

void PiServer::setPort(int portNumber){
    if ((portNumber < 65535) && (portNumber > 0)) 
        port_ = portNumber;
}
int PiServer::getPort(){
    return port_;
}

bool PiServer::startServer(){
    if (port_ == -1)
        return false;
    
    struct sockaddr_in addr;
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
     
    if (socket_ < 0){
        printf("socket error №%i\n",errno);
        return false;
    }
     
    printf("sock= %i \n",socket_);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port_);
    addr.sin_addr.s_addr= htonl(INADDR_ANY);
    const int trueFlag = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
        printf("Failure");
    if (bind(socket_,(struct sockaddr *)&addr,sizeof(addr)) == -1){
        printf("connect error  %i\n",errno);
        return false;
    }
     
    if(listen(socket_, 2) == -1){
        printf("listen() error lol!");
        return false;
    }

    portIsOpen_ = true;
    return true;
}

bool PiServer::startServer(int portNumber){
    this->setPort(portNumber);
    return this->startServer();
}

bool PiServer::getIncomingConnection(){
    sd_ = accept(socket_,NULL,NULL);
    
    if (sd_ == -1) {
        printf("error accept %i\n",errno);
        return false;
    }
    printf("Incoming socket %i", sd_);
    return true;
}

bool PiServer::portIsOpen(){
    return portIsOpen_;
}

bool PiServer::sendBuffer(const char* buff, const int size){
    int sentSize = 0;
    while (sentSize < size){
        int len = write(sd_, buff + sentSize, size - sentSize);
        if (len < 0){
            printf("%s", strerror(errno));
            return false;
        }
        sentSize += len;
    }
    return true;
}

bool PiServer::receiveBuffer(char* buff, const int size){
    int receivedSize = 0;
    while (receivedSize < size){
        int len = read(sd_, buff + receivedSize, size - receivedSize);
        if (len < 0){
            printf("receiveBuffer error %s", strerror(errno));
            return false;
        }
        if (len == 0){
            printf("Empty packet");
            break;
        }
        printf("Received receiveBuffer size %i\n", len);
        receivedSize += len;
    }
    printf("Size %i, receivedSize %i\n", size, receivedSize);
    return size == receivedSize;
}

bool PiServer::sendMessage(const char* buff, const int size){
    char messageSizeStr[5] = {0};
    sprintf(messageSizeStr, "%d", size);
    printf("%d", size);
    sendBuffer(messageSizeStr, 2);
    return sendBuffer(buff, size);
}

bool PiServer::readMessage(){
    char sizeBuff[5] = {0};
    if (receiveBuffer(sizeBuff, 2)){
        printf("Received readMessage %s\n", sizeBuff);
    } else return false;
    int size = atoi(sizeBuff);
    char* buff = new char[size];
    buff_ = "";
    
    if (receiveBuffer(buff, size)){
        buff_ = string(buff, size);
        delete[] buff;
        return true;
    }
    delete[] buff;
    return false;
}

string PiServer::getMessage(){
    return buff_;
}