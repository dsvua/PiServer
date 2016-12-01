/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AdafruitMotorHat.cpp
 * Author: serhiy
 * 
 * Created on November 29, 2016, 9:10 PM
 */

#include "AdafruitMotorHat.h"

AdafruitMotorHat::AdafruitMotorHat(int address) {
    hatAddress_ = address;
        motors_[0].pwm = 8;
        motors_[0].in1 = 9;
        motors_[0].in2 = 10;
        motors_[1].pwm = 13;
        motors_[1].in1 = 12;
        motors_[1].in2 = 11;
        motors_[2].pwm = 2;
        motors_[2].in1 = 3;
        motors_[2].in2 = 4;
        motors_[3].pwm = 7;
        motors_[3].in1 = 6;
        motors_[3].in2 = 5;
}

AdafruitMotorHat::AdafruitMotorHat(const AdafruitMotorHat& orig) {
}

AdafruitMotorHat::~AdafruitMotorHat() {
}

void AdafruitMotorHat::setSpeed(int speed, int motorId) {
    if (speed > 0) {
        //motors_[motorId].in
        printf("changing speed");
    }
}

