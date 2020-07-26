#include "main.h"

void print(gyro_raw_t& gyro_raw) 
{
	printf("#GRAW(X,Y,Z): ");
	printf("%6d, ", gyro_raw.x);
	printf("%6d, ", gyro_raw.y);
	printf("%6d ", gyro_raw.z);
}
void println(void) 
{
	printf("\n");
}
void print(gyro_offset_t& gyro_offset) 
{	
	printf("#GOFF(X,Y,Z): ");
	printf("%6d, ", gyro_offset.x);
	printf("%6d, ", gyro_offset.y);
	printf("%6d ", gyro_offset.z);
}
void print(gyro_adj_t& gyro_adj) 
{	
	printf("#GADJ(X,Y,Z): ");
	printf("%6d, ", gyro_adj.x);
	printf("%6d, ", gyro_adj.y);
	printf("%6d ", gyro_adj.z);
}
void print(gyro_rate_t& gyro_rate) 
{	
	printf("#GRATE(P,R,Y): ");
	printf("%6.1f, ", gyro_rate.pitch);
	printf("%6.1f, ", gyro_rate.roll);
	printf("%6.1f ", gyro_rate.yaw);
}
void print(dt_t& dt) 
{	
	printf("#DT:%6.6f", dt.t_period);
}
void print(gyro_angle_t& gyro_angle) 
{	
	printf("#GANGLE(P,R,Y): ");
	printf("%6.1f, ", gyro_angle.pitch);
	printf("%6.1f, ", gyro_angle.roll);
	printf("%6.1f ", gyro_angle.yaw_now);
}
void print(sonic_calc_t& sonic_calc)
{
	printf("#SONIC: ");
	printf("%6.6f, ", sonic_calc.distance_now);
	printf("#SONICother: ");
	printf("%6.6f, ", sonic_calc.distance_other);
	printf("#timep: ");
	printf("%6.6f, ", sonic_calc.pulse_start);
	printf("timen: ");
	printf("%6.6f ", sonic_calc.pulse_end);
}
void print(paint_move_t& paint_move)
{
	printf("#PMOV(X,Y): ");
	printf("%6.6f, ", paint_move.x);
	printf("%6.6f ", paint_move.y);
}
	
