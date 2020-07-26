#include "main.h"

const QImage image("cursor.png");

Drone::Drone(QWidget *parent)
    : QWidget(parent)
{
    
}
void Drone::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setViewport(0, 0, 1000, 1000);
    painter.setWindow(0, 0, 1000, 1000);
    
    draw(&painter);
}
void Drone::draw(QPainter *painter)
{
    extern mpu6050_t mpu6050;
    extern gyro_raw_t gyro_raw;
    extern gyro_offset_t gyro_offset;
    extern gyro_adj_t gyro_adj;
    extern gyro_rate_t gyro_rate;
    extern dt_t dt;
    extern gyro_angle_t gyro_angle;
    extern paint_move_t paint_move;
    extern sonic_calc_t sonic_calc;
    double minusX, minusY, moveX, moveY, angle_prev, angle_now, angle_diff;
    
    read(mpu6050, gyro_raw);
    calc(gyro_adj, gyro_raw, gyro_offset);
    calc(gyro_rate, gyro_adj);
    calc(dt);
    calc(gyro_angle, gyro_rate, dt);
    
    angle_prev = sqrt(gyro_angle.yaw_prev * gyro_angle.yaw_prev);
    angle_now = sqrt(gyro_angle.yaw_now * gyro_angle.yaw_now);
    
    if(angle_prev >= angle_now)
    {
        angle_diff = angle_prev - angle_now;
    }
    else
    {
        angle_diff = angle_now - angle_prev;
    }
    
    if(angle_diff > 3)
    {
	init2(paint_move);
	init2(sonic_calc);
	init2(gyro_angle);
    }
    
    calc(sonic_calc);
    calc(paint_move, sonic_calc, gyro_angle);
    
    moveX = 200 + paint_move.x;
    moveY = 150 + paint_move.y;
    minusX = -1 * moveX;
    minusY = -1 * moveY;
	
    QTransform transform;
    transform.translate(moveX, moveY);
    transform.rotate(gyro_angle.yaw_now);
    transform.translate(minusX, minusY);
    painter->setWorldTransform(transform);
    painter->drawImage(moveX, moveY, image);
    
    print(sonic_calc);
    println();
    delay(100);
    
    update();
}
