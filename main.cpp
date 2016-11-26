/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: serhiy
 *
 * Created on November 24, 2016, 10:07 AM
 */

#include <cstdlib>
#include "PiServer.h"
#include <string>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    PiServer piServer;
    bool res = piServer.startServer(8080);
    while (true){
        printf("res %e\n", res);
        piServer.getIncomingConnection();
        printf("Creating connection");

        while (piServer.readMessage()) {
            //piServer.readMessage();
            string message = piServer.getMessage();
            if (!message.empty() && message[message.length()-1] == '\n') {
                message.erase(message.length()-1);
            }
            if (!message.empty() && message[message.length()-1] == '\r') {
                message.erase(message.length()-1);
            }
            printf("message:%s:\n", message.c_str());
            printf("Comparison is %i", message.compare("exit"));
            if (!message.compare("exit")) break;
            if (!message.compare("respond")) piServer.sendMessage("Responded\n", 10);
        }
    }
    return 0;
}

