import RPi.GPIO as GPIO
GPIO.setwarnings(False)
from mfrc522 import SimpleMFRC522
import time

GPIO.setmode(GPIO.BOARD)
GPIO.setup(33, GPIO.OUT)
SalidaPWM1=GPIO.PWM(33,68)
SalidaPWM1.start(11.2)

reader = SimpleMFRC522()
a=54
c=68
cont=0
alerta=False

while True:
    
    try:
        id, text = reader.read()
        print(id)
       
        if id == 799748657696: 
            
            
            for x in range (100,150):
                SalidaPWM1.ChangeDutyCycle(x*(18.1/180))
                time.sleep(0.02)
            time.sleep(4)
            for x in range (100,140):
                SalidaPWM1.ChangeDutyCycle((250-x)*(18.1/180))
                time.sleep(0.02)
            alerta=True
        else:
            time.sleep(1)
    finally:
        GPIO.cleanup()
