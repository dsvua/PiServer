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
        printf("Resetting hat\n");
        piServer.PiCar.resetHat();
        //piServer.PiCar->resetHat();
        system("sudo killall gst-launch-1.0");
        printf("res %e\n", res);
        piServer.getIncomingConnection();
        printf("Creating connection");
        stringstream videoCmd;
        videoCmd << "/usr/bin/screen -L -dm /bin/bash -c "
                "'/usr/bin/raspivid -vf -hf -n "
                "-w 1024 -h 768 -b 1000000 -fps 15 -t 0 -o - | "
                "/usr/bin/gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay "
                "config-interval=10 pt=96 ! udpsink "
                "host=" << piServer.client_ipstr_ << " port=9000'";
        const string& tmp = videoCmd.str();
        const char* videoCmdTmp = tmp.c_str();
        printf("\nrunning:");
        printf(videoCmdTmp);
        printf("\n");
        system(videoCmdTmp);

        while (piServer.readMessage()) {
            
            //piServer.readMessage();
            printf("Getting message\n");
            string message = piServer.getMessage();
            printf("Parsing message\n");
            piServer.parseMessage(message);
            printf("Message:%s\n");
/*            
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
*/
            
        }
    }
    return 0;
}

