import os
import sys
import threading
import socket
import controll
import pedfinal

ctrl = controll.Control()

def run_server(port=8888):
    host = ''
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(5)
        print('run server', host, port)
        conn, addr = s.accept()
        opcv = pedfinal.Opencv(conn)
        print('accepted', addr)

        thread_detect = threading.Thread(target=opcv.detect_human)
        print("Human detecting...")

        thread_detect.start()
        #thread_detect.join()
        
        while True:
            msg = conn.recv(1024)
            if not msg.decode():
                break

            if msg.decode() == 'forward':
                ctrl.FWD()
            elif msg.decode() == 'back':
                ctrl.BACK()
            elif msg.decode() == 'right':
                ctrl.RIGHT()
            elif msg.decode() == 'left':
                ctrl.LEFT()
            elif msg.decode() == 'stop':
                ctrl.STOP1()
            elif msg.decode() == 'auto_on':
                thread_driver = threading.Thread(target=ctrl.loop_driver, args=(0,0))
                print("auto drive start")

                thread_driver.start()
                #thread_driver.join()
            elif msg.decode() == 'auto_off':
                ctrl.loop = False
                print("auto off")

    ctrl.loop = False
    opcv.detecting = False
    conn.close()

#def thread_stream():
    #while True:
        #os.system('sh /home/pi/mjpg.sh')
        
#t1 = threading.Thread(target = thread_stream)
#t1.start()

if __name__ == '__main__':
    run_server()

ctrl.STOP1()
print('end')
