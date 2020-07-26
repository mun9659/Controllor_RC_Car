import numpy as np
import cv2
#from matplotlib import pyplot as plt
class Opencv():
    def __init__(self, conn):
        self.conn = conn
        self.video = cv2.VideoCapture("http://192.168.43.193:8080/stream/video.mjpeg")
        self.detped = cv2.CascadeClassifier('haarcascade_fullbody.xml')

        #cv2.destroyAllWindows()

    def detect_human(self):
        self.video.set(1, 1) # cam frame
        self.video.set(3, 320) # cam width 
        self.video.set(4, 240) # cam height

        self.detecting = True

        while self.detecting:
            ret, frame = self.video.read()

            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            ped = self.detped.detectMultiScale(gray, 1.1, 4)

            print(ped)
            try:
                if ped[0] != None:
                     self.conn.send("Human Detect!")
            except:
                continue
        
        
            cv2.imshow('video', frame)

            if cv2.waitKey(25) & 0xFF == ord('q'):
                break

        self.video.release()
        

