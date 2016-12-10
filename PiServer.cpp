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

PiServer::PiServer()
    : PiCar(HAT_ADDRESS)
{
    port_ = -1;
    portIsOpen_ = false;
    speed_ = 800;
    
}
/*
PiServer::PiServer(const PiServer& orig)
{
}
*/
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
        printf("listen() error!");
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
    
    //----------------------
    struct sockaddr     address;
    socklen_t           addresslen;
    struct sockaddr_in* addrInternet;
    string              ip;
    int port;
    char caddr[INET_ADDRSTRLEN];

    addresslen = sizeof(address);
    getpeername(sd_, &address, &addresslen);
    addrInternet = (struct sockaddr_in*)&address;
    
    port = ntohs(addrInternet->sin_port);
    inet_ntop(AF_INET, &(addrInternet->sin_addr), caddr, INET_ADDRSTRLEN);
    client_ipstr_ = inet_ntoa(addrInternet->sin_addr);
    
    //std::copy(std::begin(ipstr), std::end(ipstr), std::begin(client_ipstr_));
    printf("Peer IP address ntoa: %s\n", client_ipstr_.c_str());
    printf("Peer IP address ntop: %s\n", caddr);
    printf("Peer port      : %d\n", port);
    //----------------------
   
    
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

void PiServer::parseMessage(string message){
    vector<string> splitted_message = splitMessage(message, ':');
    int value = atoi(splitted_message[1].c_str());
    //value = atoi(strtok(cmessage, ":"));
    //value = atoi(cmessage);
    string command = splitted_message[0];
    cout << message << "|" << splitted_message[0] << " " << value << "|" << splitted_message[1] << endl;
    if (command == "Reset"){
        PiCar.resetHat();
    } else if (command == "Forward"){
        if (speed_ < (-1) * SPEED_MIN) PiCar.setDirection(DIRECTION_STOP);
        else if (speed_ < SPEED_MIN) PiCar.setDirection(DIRECTION_FORWARD);
        speed_ += value;
        if (speed_ > 4096) speed_ = 4096;
        cout << "Setting speed Forward" << speed_ << endl;
        PiCar.setSpeed(speed_);
    } else if (command == "Backward"){
        if (speed_ > SPEED_MIN) PiCar.setDirection(DIRECTION_STOP);
        else if (speed_ > (-1) * SPEED_MIN) PiCar.setDirection(DIRECTION_BACKWARD);
        speed_ -= value;
        if (speed_ < -4096) speed_ = -4096;
        PiCar.setSpeed(speed_);
    } else if (command == "Right"){
        PiCar.setSpeedDiff(value);
        PiCar.setSpeed(speed_);
    } else if (command == "Left"){
        PiCar.setSpeedDiff(value * (-1));
        PiCar.setSpeed(speed_);
    }
    //delete cmessage;
    
    
}

void PiServer::split(const string &s, char delim, vector<string> &elems){
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> PiServer::splitMessage(const string &s, char delim){
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}