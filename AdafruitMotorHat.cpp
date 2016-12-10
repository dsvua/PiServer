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

AdafruitMotorHat::AdafruitMotorHat(const int address) {
    hatAddress_    = address;  // by default 0x60 motor hat and 0x40 servo
    direction_     = DIRECTION_STOP;
    motors_[0].pwm = 8;
    motors_[0].in1 = 10;
    motors_[0].in2 = 9;
    motors_[0].speedCoef = 1;
    motors_[1].pwm = 13;
    motors_[1].in1 = 11;
    motors_[1].in2 = 12;
    motors_[1].speedCoef = 1;
    motors_[2].pwm = 2;
    motors_[2].in1 = 4;
    motors_[2].in2 = 3;
    motors_[2].speedCoef = 1;
    motors_[3].pwm = 7;
    motors_[3].in1 = 5;
    motors_[3].in2 = 6;
    motors_[3].speedCoef = 1;
    
    //------------------------
    printf("Opening /dev/i2c-1\n");
    shield_fd_ = open("/dev/i2c-1", O_RDWR);
    printf("Shield is set with fd %d\n", shield_fd_);
    setI2CAddr(shield_fd_, hatAddress_);
    //------------------------
/*    
    setAllPwm(0, 0);
    writeReg8(PCA9685_MODE2, PCA9685_OUTDRV);
    writeReg8(PCA9685_MODE1, PCA9685_ALLCALL);
    sleep(1);
    int mode1 = readReg8(PCA9685_MODE1);
    mode1 = mode1 & PCA9685_SLEEP;
    writeReg8(PCA9685_MODE1, mode1);
    sleep(1);
    prescale    = (int)(25000000.0f / (4096 * FREQ) - 0.5f);
    int oldMode = readReg8(PCA9685_MODE1);
    int newMode = (oldMode & 0x7F) | 0x10;
    writeReg8(PCA9685_MODE1, newMode);
    writeReg8(PCA9685_PRESCALE, prescale);
    writeReg8(PCA9685_MODE1, oldMode);
    sleep(1);
    writeReg8(PCA9685_MODE1, oldMode | 0x80);
*/    
    settings    = readReg8(PCA9685_MODE1) & 0x7F;
    autoInc     = settings | 0x20;
    prescale    = (int)(25000000.0f / (4096 * FREQ) - 0.5f);
    sleep_	= settings | 0x10;
    wake        = settings & 0xEF;
    restart     = wake | 0x80;
    writeReg8(PCA9685_MODE1, autoInc);
    writeReg8(PCA9685_MODE1, sleep_);
    writeReg8(PCA9685_PRESCALE, prescale);
    writeReg8(PCA9685_MODE1, wake);
    sleep(1);
    writeReg8(PCA9685_MODE1, restart);
/**/    
    speedDiff_ = 0; // if > 0, left wheels turn faster
    if (shield_fd_ < 0){
        printf("Error in i2c setup\n");
    }
    //resetHat();
    //printf("Shield was reset\n");
}

//AdafruitMotorHat::AdafruitMotorHat(const AdafruitMotorHat& orig) {
//}

AdafruitMotorHat::~AdafruitMotorHat() {
    close(shield_fd_);
}

int AdafruitMotorHat::baseReg(int pin){
    return (pin >= PIN_ALL ? LEDALL_ON_L : LED0_ON_L + 4 * pin);
}

void AdafruitMotorHat::setSpeed(int speed) {
    // speed is a number 0..100 or -100..0
    int tmp_in1 = 0;
    int tmp_in2 = 0;
    for (int i = 0; i < 4; ++i){
        printf("Motor id %d \n", i);
        int mSpeed = static_cast<int>(speed * motors_[i].speedCoef);
        printf ("initial mSpeed=%d\n", mSpeed);

        if (i == 0 || i == 3){ // left motors
            if (speedDiff_ < 0) mSpeed -= abs(speedDiff_);
        } else {
            if (speedDiff_ > 0) mSpeed -= abs(speedDiff_);
        }
        switch (direction_){
            case DIRECTION_FORWARD:
                if (mSpeed < 0) mSpeed = 0;
                break;
            case DIRECTION_BACKWARD:
                if (mSpeed > 0) mSpeed = 0;
                break;
            case DIRECTION_STOP:
                mSpeed = 0;
                break;
        }
        setPwm(motors_[i].pwm, 0, 
                    static_cast<int>(mSpeed));
        
        printf("Speed %d, mSpeed %d\n", speed, mSpeed);
    }
    printf("\n");
}

void AdafruitMotorHat::setDirection(int direction) {
    direction_ = direction;
    for (int i = 0; i < 4; ++i){
        printf("Direction for Motor id %d \n", i);
        switch (direction_){
            case DIRECTION_FORWARD:
                setFullOn(motors_[i].in1);
                setFullOff(motors_[i].in2);
                break;
            case DIRECTION_BACKWARD:
                setFullOff(motors_[i].in1);
                setFullOn(motors_[i].in2);
                break;
            case DIRECTION_STOP:
                setFullOff(motors_[i].in1);
                setFullOff(motors_[i].in2);
                break;
        }
    }
}

void AdafruitMotorHat::resetHat(){
    printf("Called AdafruitMotorHat::resetHat\n");
    setAllPwm(0, 0);
}

void AdafruitMotorHat::setSpeedDiff(int diff_) {
    speedDiff_ = diff_;
}

void AdafruitMotorHat::setPwm(int pin, int pwm_on, int pwm_off){
    printf("setPwm pin %d, on %d, off %d\n", pin, pwm_on, pwm_off);
    if (pwm_on > 4096) pwm_on = 4096;
    if (pwm_on < 0) pwm_on = 0;
    if (pwm_off > 4096) pwm_off = 4096;
    if (pwm_off < 0) pwm_off = 0;
    
    int reg = baseReg(pin);
    writeReg8(reg, pwm_on & 0xFF);
    writeReg8(reg + 1, pwm_on >> 8);
    writeReg8(reg + 2, pwm_off & 0xFF);
    writeReg8(reg + 3, pwm_off >> 8);
}

void AdafruitMotorHat::setFullOn(int pin){
    setPwm(pin, 4096, 0);
    //int reg = baseReg(pin);
    //writeReg8(reg + 1, 0x10);
    //writeReg8(reg + 3, 0);
}

void AdafruitMotorHat::setFullOff(int pin){
    setPwm(pin, 0, 4096);
}
void AdafruitMotorHat::setFd(int fd){
    shield_fd_ = fd;
}

int AdafruitMotorHat::readReg8(uint8_t reg){
    setI2CAddr(shield_fd_, hatAddress_);
    int res = i2c_smbus_read_byte_data(shield_fd_, reg);
    usleep(delay_);
    return res;
}

int AdafruitMotorHat::writeReg8(uint8_t reg, uint8_t value){
    printf("writeReg8: fd %d, slave %d, addr %d, reg %d, value %d\n", 
            shield_fd_, I2C_SLAVE, hatAddress_, reg, value);
    setI2CAddr(shield_fd_, hatAddress_);
    int res = i2c_smbus_write_byte_data(shield_fd_, reg, value);
    usleep(delay_);
    if (res < 0) printf("Error writeReg8");
    return res;
}

void AdafruitMotorHat::setAllPwm(int pwm_on, int pwm_off){
    if (pwm_on > 4096) pwm_on = 4096;
    if (pwm_on < 0) pwm_on = 0;
    if (pwm_off > 4096) pwm_off = 4096;
    if (pwm_off < 0) pwm_off = 0;
    writeReg8(LEDALL_ON_L,     pwm_on & 0xFF);
    writeReg8(LEDALL_ON_L + 1, pwm_on >> 8);
    writeReg8(LEDALL_ON_L + 2, pwm_off & 0xFF);
    writeReg8(LEDALL_ON_L + 3, pwm_off >> 8);
}

void AdafruitMotorHat::setI2CAddr(int fd, int addr){
    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    //usleep(delay_);
}
