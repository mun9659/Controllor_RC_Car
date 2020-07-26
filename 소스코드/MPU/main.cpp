#include "main.h"

i2c_t i2c;
mpu6050_t mpu6050 = 
{
	.i2c_addr = 0x68,
	.PWR_MGMT_1 = 0x6b,
	.GYRO_XOUT_H = 0x43,
};
gyro_raw_t gyro_raw;
gyro_offset_t gyro_offset;
gyro_adj_t gyro_adj;
gyro_rate_t gyro_rate;
dt_t dt;
gyro_angle_t gyro_angle;
paint_move_t paint_move;
sonic_calc_t sonic_calc;

int main(int argc, char *argv[])
{	
	wiringPiSetup();
	init(i2c);
	init(i2c, mpu6050);
	get(mpu6050, gyro_offset);
	init(dt);
	init(gyro_angle);
	init(sonic_calc);
	init(paint_move);
	
	QApplication app(argc, argv);
	Drone drone;
	drone.show();
	return app.exec();
	
	/*while(true)
	{	
		read(mpu6050, gyro_raw);
		calc(gyro_adj, gyro_raw, gyro_offset);
		calc(gyro_rate, gyro_adj);
		calc(dt);
		calc(gyro_angle, gyro_rate, dt);
		calc(sonic_calc);
		calc(paint_move, sonic_calc, gyro_angle);
		
		//print(gyro_raw);
		//print(gyro_offset);
		//print(gyro_adj);
		//print(gyro_rate);
		//print(dt);
		//print(gyro_angle);
		//print(sonic_calc);
		//print(paint_move);
		//println();
	}*/
}
