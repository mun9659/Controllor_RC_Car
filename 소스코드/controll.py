#import controll
import RPi.GPIO as GPIO
import time
import random

class Control():
    def __init__(self):

        global pwmA, pwmB

        #MOTOR STATUS
        self.STOP = 0
        self.FORWARD = 1
        self.BACKWARD = 2

        #PIN high, low
        self.HIGH = 1
        self.LOW = 0

        #PWM PIN Number
        self.ENB = 0
        self.ENA = 26

        #GPIO PIN Number
        self.IN4 = 5
        self.IN3 = 6
        self.IN2 = 13
        self.IN1 = 19

        #MOTOR Channel
        self.CH1 = 0
        self.CH2 = 1

        #SPEED
        self.speed_go = 28
        self.speed_back = 33
        self.speed_turn = 67

        #ECHO PIN Number
        self.rig_trig=27
        self.rig_echo=22

        self.str_trig=18
        self.str_echo=17

        self.lef_trig=24
        self.lef_echo=23
        
        GPIO.cleanup()
        GPIO.setmode(GPIO.BCM)

        GPIO.setup(self.IN1, GPIO.OUT)
        GPIO.setup(self.IN2, GPIO.OUT)
        GPIO.setup(self.ENA, GPIO.OUT)

        GPIO.setup(self.IN3, GPIO.OUT)
        GPIO.setup(self.IN4, GPIO.OUT)
        GPIO.setup(self.ENB, GPIO.OUT)

        GPIO.setup(self.str_trig, GPIO.OUT)
        GPIO.setup(self.str_echo, GPIO.IN)
        GPIO.setup(self.rig_trig, GPIO.OUT)
        GPIO.setup(self.rig_echo, GPIO.IN)
        GPIO.setup(self.lef_trig, GPIO.OUT)
        GPIO.setup(self.lef_echo, GPIO.IN)
        
        pwmA = GPIO.PWM(self.ENA, 300) #MOTOR 1
        pwmA.start(0)

        pwmB = GPIO.PWM(self.ENB, 300) #MOTOR 2
        pwmB.start(0)

    def distance_check(self, trig, echo):

        GPIO.output(trig, GPIO.LOW)
        time.sleep(0.25)
        
        GPIO.output(trig, GPIO.HIGH)
        time.sleep(0.00001)
        GPIO.output(trig, GPIO.LOW)
        stop=0
        start=0
        while GPIO.input(echo) == GPIO.LOW:
            start = time.time()

        while GPIO.input(echo) == GPIO.HIGH:
            stop = time.time()

        duration = stop-start
        distance = (duration*340*100)/2
        distance = round(distance, 2)
        
        return distance

    def loop_driver(self, nloop, nsec):

        self.loop = True

        while self.loop:
            str_dt = self.distance_check(self.str_trig, self.str_echo)
            rig_dt = self.distance_check(self.rig_trig, self.rig_echo)
            lef_dt = self.distance_check(self.lef_trig, self.lef_echo)


            if str_dt > 50:
                str_dt = 50
            if rig_dt > 100:
                rig_dt = 100
            if lef_dt > 100:
                lef_dt = 100
            
            print("Str Dt : ", str_dt, "cm ", "Rig Dt : ", rig_dt, "cm ", "Lef Dt : ", lef_dt, "cm ")
            #print("Rig Dt : ", rig_dt, "cm\n")
            #print("Lef Dt : ", lef_dt, "cm\n")

            if str_dt<30:
                self.STOP1()
                time.sleep(1)
                
                if rig_dt>lef_dt:
                    self.BACK()
                    time.sleep(0.1)
                    self.RIGHT()
                    #time.sleep(0.5)
                    time.sleep(0.15)

                    
                elif rig_dt<lef_dt:
                    self.BACK()
                    time.sleep(0.1)
                    self.LEFT()
                    #time.sleep(0.5)
                    time.sleep(0.15)

                else:
                    self.BACK()
                    time.sleep(0.1)
                    

            else:
                self.FWD()

        self.loop = False
        print("end auto drive")
        self.STOP1()
                

    def setMotorControl(self, pwm, INA, INB, stat):
        if stat == self.FORWARD:
            GPIO.output(INB, self.HIGH)
            GPIO.output(INA, self.LOW)
        elif stat == self.BACKWARD:
            GPIO.output(INB, self.LOW)
            GPIO.output(INA, self.HIGH)
        elif stat == self.STOP:
            GPIO.output(INB, self.LOW)
            GPIO.output(INA, self.LOW)

    def setMotor(self, ch, stat):
        if ch == self.CH1:
            self.setMotorControl(pwmA, self.IN1, self.IN2, stat)
        else:
            self.setMotorControl(pwmB, self.IN3, self.IN4, stat)

    def FWD(self):
        print("FORWARD")
        pwmA.ChangeDutyCycle(self.speed_go)
        pwmB.ChangeDutyCycle(self.speed_go)
        self.setMotor(self.CH1, self.FORWARD)
        self.setMotor(self.CH2, self.FORWARD)

    def BACK(self):
        print("BACKWARD")
        pwmA.ChangeDutyCycle(self.speed_back)
        pwmB.ChangeDutyCycle(self.speed_back)
        self.setMotor(self.CH1, self.BACKWARD)
        self.setMotor(self.CH2, self.BACKWARD)

    def RIGHT(self):
        print("RIGHT")
        pwmA.ChangeDutyCycle(self.speed_turn)
        pwmB.ChangeDutyCycle(self.speed_turn)
        self.setMotor(self.CH1, self.FORWARD)
        self.setMotor(self.CH2, self.BACKWARD)

    def LEFT(self):
        print("LEFT")
        pwmA.ChangeDutyCycle(self.speed_turn)
        pwmB.ChangeDutyCycle(self.speed_turn)
        self.setMotor(self.CH1, self.BACKWARD)
        self.setMotor(self.CH2, self.FORWARD)

    def STOP1(self):
        print("STOP")
        pwmA.ChangeDutyCycle(0)
        pwmB.ChangeDutyCycle(0)
        self.setMotor(self.CH1, self.STOP)
        self.setMotor(self.CH2, self.STOP)
