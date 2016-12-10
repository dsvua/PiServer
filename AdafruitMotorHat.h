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
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <iostream>

// Setup registers
#define PCA9685_MODE1    0x0
#define PCA9685_MODE2    0x1
#define PCA9685_PRESCALE 0xFE
// Setup bits
#define PCA9685_RESTART  0x80
#define PCA9685_SLEEP    0x10
#define PCA9685_ALLCALL  0x01
#define PCA9685_INVRT    0x10
#define PCA9685_OUTDRV   0x04

// Define first LED and all LED. We calculate the rest
#define LED0_ON_L 0x6
#define LEDALL_ON_L 0xFA

#define PIN_ALL 16

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 1000
#define SPEED_MAX 4096
#define SPEED_MIN 1000
#define FREQ 1600

#define DIRECTION_FORWARD 1
#define DIRECTION_BACKWARD 2
#define DIRECTION_STOP 0

/*
// I2C definitions
#define I2C_SLAVE       0x0703
#define I2C_SMBUS       0x0720  // SMBus-level access
#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

// SMBus transaction types
#define I2C_SMBUS_QUICK             0
#define I2C_SMBUS_BYTE              1
#define I2C_SMBUS_BYTE_DATA         2
#define I2C_SMBUS_WORD_DATA         3
#define I2C_SMBUS_PROC_CALL         4
#define I2C_SMBUS_BLOCK_DATA        5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7
#define I2C_SMBUS_I2C_BLOCK_DATA    8

// SMBus messages
#define I2C_SMBUS_BLOCK_MAX     32      // As specified in SMBus standard
#define I2C_SMBUS_I2C_BLOCK_MAX 32      // Not specified but we use same structure
*/
/*
// Structures used in the ioctl() calls
union i2c_smbus_data{
    uint8_t  byte;
    uint16_t word;
    uint8_t  block [I2C_SMBUS_BLOCK_MAX + 2];    // block [0] is used for length + one more for PEC
};

struct i2c_smbus_ioctl_data{
    char read_write;
    uint8_t command;
    int size;
    union i2c_smbus_data *data;
};
*/


class AdafruitMotorHat {
public:
    AdafruitMotorHat(const int address);
    AdafruitMotorHat(const AdafruitMotorHat& orig) = default;
    virtual ~AdafruitMotorHat();
    void setSpeed(int speed);
    void setDirection(int direction);
    void setSpeedDiff(int diff_);
    void resetHat();
    int baseReg(int pin);
    void setPwm(int pin, int pwm_on, int pwm_off);
    void setAllPwm(int pwm_on, int pwm_off);
    void setFullOn(int pin);
    void setFullOff(int pin);
    void setFd(int fd);
    void setI2CAddr(int fd, int addr);
    //int i2c_smbus_access (char rw, uint8_t command, int size, 
    //        union i2c_smbus_data *data);
    int readReg8(uint8_t reg);
    int writeReg8(uint8_t reg, uint8_t value);
    
private:
    struct motor_ {
        int pwm;
        int in1;
        int in2;
        float speedCoef;
    };
    int hatAddress_;
    int shield_fd_;
    motor_ motors_[4];
    int speedDiff_;
    int prescale;
    int sleep_;
    int wake;
    int restart;
    int settings;
    int autoInc;
    int direction_;
    useconds_t delay_ = 2000;

};

#endif /* ADAFRUITMOTORHAT_H */

