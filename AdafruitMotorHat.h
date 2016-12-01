/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AdafruitMotorHat.h
 * Author: serhiy
 *
 * Created on November 29, 2016, 9:10 PM
 */

#ifndef ADAFRUITMOTORHAT_H
#define ADAFRUITMOTORHAT_H

#include <stdio.h>

class AdafruitMotorHat {
public:
    AdafruitMotorHat(int address);
    AdafruitMotorHat(const AdafruitMotorHat& orig);
    virtual ~AdafruitMotorHat();
    void setSpeed(int speed, int motorId);
private:
    struct motor_ {
        int pwm;
        int in1;
        int in2;
    };
    int hatAddress_;
    motor_ motors_[4];
    

};

#endif /* ADAFRUITMOTORHAT_H */

