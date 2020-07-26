#include "main.h"

void init(i2c_t& i2c) 
{
	i2c.port = open("/dev/i2c-1", O_RDWR);
	if(i2c.port < 0) 
	{
		fprintf(stderr,
				"Unable to open i2c-1 device: %s\n",
				strerror(errno));
		exit(-1);
	}
}
void init(i2c_t& i2c, mpu6050_t& mpu6050) 
{
	mpu6050.i2c_port = i2c.port;
	
	ioctl(mpu6050.i2c_port, I2C_SLAVE, mpu6050.i2c_addr);
	
	wiringPiI2CWriteReg8(mpu6050.i2c_port, mpu6050.PWR_MGMT_1, 0);
}
void read(mpu6050_t& mpu6050, gyro_raw_t& gyro_raw) 
{	
	const int I2C_PORT = mpu6050.i2c_port;
	const int GYRO_XH = mpu6050.GYRO_XOUT_H;
	
	ioctl(I2C_PORT, I2C_SLAVE, mpu6050.i2c_addr);
	
	gyro_raw.x=(wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+0)&0xFF)<<8;
	gyro_raw.x|=wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+1)&0xFF;
	gyro_raw.y=(wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+2)&0xFF)<<8;
	gyro_raw.y|=wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+3)&0xFF;
	gyro_raw.z=(wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+4)&0xFF)<<8;
	gyro_raw.z|=wiringPiI2CReadReg8(I2C_PORT, GYRO_XH+5)&0xFF;
}
#define NSAMPLES 1000
void get(mpu6050_t& mpu6050, gyro_offset_t& gyro_offset) 
{		
	gyro_raw_t gyro_raw;
	
	int32_t sumGyX = 0, sumGyY = 0, sumGyZ = 0;
	
	for(int i=0;i<NSAMPLES;i++) 
	{
		read(mpu6050, gyro_raw);
		
		sumGyX += gyro_raw.x;
		sumGyY += gyro_raw.y;
		sumGyZ += gyro_raw.z;
		
		delay(1);
	}
	
	gyro_offset.x = (double)sumGyX/NSAMPLES;
	gyro_offset.y = (double)sumGyY/NSAMPLES;
	gyro_offset.z = (double)sumGyZ/NSAMPLES;
}
void calc(gyro_adj_t& gyro_adj, gyro_raw_t& gyro_raw, gyro_offset_t& gyro_offset) 
{
	gyro_adj.x = gyro_raw.x - gyro_offset.x;
	gyro_adj.y = gyro_raw.y - gyro_offset.y;
	gyro_adj.z = gyro_raw.z - gyro_offset.z;
}
void calc(gyro_rate_t& gyro_rate, gyro_adj_t& gyro_adj) 
{
	gyro_rate.roll = gyro_adj.y/131.0;
	gyro_rate.pitch = gyro_adj.x/131.0;
	gyro_rate.yaw = gyro_adj.z/131.0;
}
void init(dt_t& dt) 
{
	dt.t_prev = micros();
}
void calc(dt_t& dt) 
{
	dt.t_now = micros();
	dt.t_period = (dt.t_now - dt.t_prev)/1000000.0;
	dt.t_prev = dt.t_now;
}
void init(gyro_angle_t& gyro_angle)
{
	gyro_angle.pitch = 0;
	gyro_angle.roll = 0;
	gyro_angle.yaw_now = 0;
	gyro_angle.yaw_prev = 0;
}
void init2(gyro_angle_t& gyro_angle)
{
	gyro_angle.yaw_prev = gyro_angle.yaw_now;
}
void calc(gyro_angle_t& gyro_angle, gyro_rate_t& gyro_rate, dt_t& dt) 
{	
	gyro_angle.roll += gyro_rate.roll * dt.t_period;
	gyro_angle.pitch += gyro_rate.pitch * dt.t_period;
	gyro_angle.yaw_now += gyro_rate.yaw * dt.t_period;
}
void init(paint_move_t& paint_move)
{
	paint_move.x = 0;
	paint_move.y = 0;
	paint_move.initx = 0;
	paint_move.inity = 0;
}
void init2(paint_move_t& paint_move)
{
	paint_move.initx = paint_move.x;
	paint_move.inity = paint_move.y;
}
#define pi 3.14159265359
void calc(paint_move_t& paint_move, sonic_calc_t& sonic_calc, gyro_angle_t& gyro_angle)
{
	float move;
	double SN, CSN;
	
	SN = std::sin(gyro_angle.yaw_now * (pi / 180));
    CSN = std::cos(gyro_angle.yaw_now * (pi / 180));
	
	move = sonic_calc.distance_now - sonic_calc.distance_prev;
	
	if (move < 400)
	{
		paint_move.x = paint_move.initx + (move * CSN);
		paint_move.y = paint_move.inity + (move * SN);
	}
}
void init(sonic_calc_t& sonic_calc)
{
	sonic_calc.trig = 0;
	sonic_calc.echo = 1;
	
	pinMode(sonic_calc.trig, OUTPUT);
	pinMode(sonic_calc.echo, INPUT);
	
	digitalWrite(sonic_calc.trig, 0);
	digitalWrite(sonic_calc.trig, 1);
	delayMicroseconds(10);
	digitalWrite(sonic_calc.trig, 0);
	
	while(digitalRead(sonic_calc.echo) == 0)
	{
		sonic_calc.pulse_start = micros();
	}
	while(digitalRead(sonic_calc.echo) == 1)
	{
		sonic_calc.pulse_end = micros();
	}
	
	sonic_calc.distance_prev = (sonic_calc.pulse_end - sonic_calc.pulse_start) / 58;
	sonic_calc.distance_now = sonic_calc.distance_prev;
	
	if(sonic_calc.distance_prev > 400 || sonic_calc.distance_prev < 0)
	{
		init(sonic_calc);
	}
}
void init2(sonic_calc_t& sonic_calc)
{
	digitalWrite(sonic_calc.trig, 0);
	digitalWrite(sonic_calc.trig, 1);
	delayMicroseconds(10);
	digitalWrite(sonic_calc.trig, 0);
	
	while(digitalRead(sonic_calc.echo) == 0)
	{
		sonic_calc.pulse_start = micros();
	}
	while(digitalRead(sonic_calc.echo) == 1)
	{
		sonic_calc.pulse_end = micros();
	}
	
	sonic_calc.distance_prev = (sonic_calc.pulse_end - sonic_calc.pulse_start) / 58;
	sonic_calc.distance_now = sonic_calc.distance_prev;
	sonic_calc.distance_other = sonic_calc.distance_prev;
	
	if(sonic_calc.distance_prev > 400 || sonic_calc.distance_prev < 0)
	{
		init2(sonic_calc);
	}
}
void calc(sonic_calc_t& sonic_calc)
{
	sonic_calc.distance_other = sonic_calc.distance_now;
	
	digitalWrite(sonic_calc.trig, 0);
	digitalWrite(sonic_calc.trig, 1);
	delayMicroseconds(10);
	digitalWrite(sonic_calc.trig, 0);
	
	while(digitalRead(sonic_calc.echo) == 0)
	{
		sonic_calc.pulse_start = micros();
	}
	while(digitalRead(sonic_calc.echo) == 1)
	{
		sonic_calc.pulse_end = micros();
	}
	
	sonic_calc.distance_now = (sonic_calc.pulse_end - sonic_calc.pulse_start) / 58;
	
	float sonic_o, sonic_n, sonic_diff;
	
	sonic_o = sqrt(sonic_calc.distance_other * sonic_calc.distance_other);
	sonic_n = sqrt(sonic_calc.distance_now * sonic_calc.distance_now);
	
	if(sonic_o > sonic_n)
	{
		sonic_diff = sonic_o - sonic_n;
	}
	else if(sonic_o < sonic_n)
	{
		sonic_diff = sonic_n - sonic_o;
	}
	else
	{
		sonic_diff = 0;
	}
	
	if(sonic_calc.distance_now > 400 || sonic_calc.distance_now < 0)
	{
		calc2(sonic_calc);
	}
	
	if(sonic_diff > 20)
	{
		calc2(sonic_calc);
	}
}
void calc2(sonic_calc_t& sonic_calc)
{
	digitalWrite(sonic_calc.trig, 0);
	digitalWrite(sonic_calc.trig, 1);
	delayMicroseconds(10);
	digitalWrite(sonic_calc.trig, 0);
	
	while(digitalRead(sonic_calc.echo) == 0)
	{
		sonic_calc.pulse_start = micros();
	}
	while(digitalRead(sonic_calc.echo) == 1)
	{
		sonic_calc.pulse_end = micros();
	}
	
	sonic_calc.distance_now = (sonic_calc.pulse_end - sonic_calc.pulse_start) / 58;
	
	float sonic_o, sonic_n, sonic_diff;
	
	sonic_o = sqrt(sonic_calc.distance_other * sonic_calc.distance_other);
	sonic_n = sqrt(sonic_calc.distance_now * sonic_calc.distance_now);
	
	if(sonic_o > sonic_n)
	{
		sonic_diff = sonic_o - sonic_n;
	}
	else if(sonic_o < sonic_n)
	{
		sonic_diff = sonic_n - sonic_o;
	}
	else
	{
		sonic_diff = 0;
	}
	
	if(sonic_calc.distance_now > 400 || sonic_calc.distance_now < 0)
	{
		calc2(sonic_calc);
	}
	
	if(sonic_diff > 20)
	{
		calc2(sonic_calc);
	}
}
