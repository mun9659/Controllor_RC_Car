#ifndef DRONE_H
#define DRONE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <cmath>

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

#define I2C_SLAVE 0x0703

typedef struct { int port; } i2c_t;
typedef struct { uint8_t i2c_addr, PWR_MGMT_1, GYRO_XOUT_H, i2c_port; } mpu6050_t;
typedef struct { int16_t x, y, z; } gyro_raw_t;
typedef struct { int16_t x, y, z; } gyro_offset_t;
typedef struct { int16_t x, y, z; } gyro_adj_t;
typedef struct { double roll, pitch, yaw; } gyro_rate_t;
typedef struct { unsigned long t_prev, t_now; double t_period; } dt_t;
typedef struct { double roll, pitch, yaw_now, yaw_prev; } gyro_angle_t;
typedef struct { double x, y, initx, inity; } paint_move_t;
typedef struct { float distance_now, distance_other, distance_prev, pulse_start, pulse_end; int trig, echo; } sonic_calc_t;

class Drone : public QWidget
{
    Q_OBJECT

public:
	Drone(QWidget *parent = 0);
	
	void draw(QPainter *painter);

protected:
	void paintEvent(QPaintEvent *event);
};

void init(i2c_t&);
void init(i2c_t&, mpu6050_t&);
void read(mpu6050_t&, gyro_raw_t&);
void get(mpu6050_t&, gyro_offset_t&);
void calc(gyro_adj_t&, gyro_raw_t&, gyro_offset_t&);
void calc(gyro_rate_t&, gyro_adj_t&);
void init(dt_t&);
void calc(dt_t&);
void init(gyro_angle_t&);
void init2(gyro_angle_t&);
void calc(gyro_angle_t&, gyro_rate_t&, dt_t&);
void init(paint_move_t&);
void init2(paint_move_t&);
void calc(paint_move_t&, sonic_calc_t&, gyro_angle_t&);
void init(sonic_calc_t&);
void init2(sonic_calc_t&);
void calc(sonic_calc_t&);
void calc2(sonic_calc_t&);

void print(gyro_raw_t&);
void println(void);
void print(gyro_offset_t&);
void print(gyro_adj_t&);
void print(gyro_rate_t&);
void print(dt_t&);
void print(gyro_angle_t&);
void print(sonic_calc_t&);
void print(paint_move_t&);

#endif // DRONE_H
