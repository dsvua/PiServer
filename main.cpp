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

int startVideo(string ip_address){
    stringstream videoCmd;
    videoCmd << "host=" << ip_address;
    const string& tmp = videoCmd.str();
    const char* cTmp = tmp.c_str();
    char *program = "/usr/bin/gst-launch-1.0";
    pid_t child_pid;
    child_pid = fork ();
    if (child_pid != 0)
        // This is the parent process.
        return child_pid;
    else {
        // Now execute PROGRAM, searching for it in the path.
        execlp("gst-launch-1.0", "gst-launch-1.0", "rpicamsrc", "bitrate=1000000", 
                "rotation=180" , "!", "video/x-h264,width=640,height=480",
                "!", "h264parse", "!", "queue", "!", 
                "rtph264pay", "config-interval=1", "pt=96", "!",
                "udpsink", "host=192.168.192.13", "port=9000", (char *)NULL);
    }
}

int main(int argc, char** argv) {
    PiServer piServer;
    bool res = piServer.startServer(8080);
    while (true){
        printf("Resetting hat\n");
        piServer.PiCar.resetHat();
        //piServer.PiCar->resetHat();
        system("sudo killall gst-launch-1.0");
        printf("res %e\n", res);
        printf("Creating connection\n");
        piServer.getIncomingConnection();
        printf("Starting video\n");
        printf("/usr/bin/gst-launch-1.0 rpicamsrc bitrate=1000000" 
                " rotation=180 ! video/x-h264,width=640,height=480 "
                "! h264parse ! queue ! rtph264pay config-interval=1"
                " pt=96 ! udpsink host=192.168.192.13 port=9000\n");
        startVideo(piServer.client_ipstr_);
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

