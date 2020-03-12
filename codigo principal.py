# -*- coding: utf-8 -*-
"""
Created on Mon Mar  9 14:47:16 2020

@author: StivenAP
"""
import sys
import time
import paho.mqtt.publish as publish
import paho.mqtt.client
from PyQt5 import uic, QtWidgets#, QtCore, QtGui

#Variables globales
bandera1 = 0
bandera2 = 0
bandera3 = 0

#Direccionamiento
topic  ='laboratorios/lab1/sensor1'
topic1 ='laboratorios/lab1/sensor3'
topic2 ='laboratorios/lab1/sensor2'
topic3 ="laboratorios/lab1/receptor1"
topic4 ="laboratorios/lab1/receptor2"

#Sub  

    
client = paho.mqtt.client.Client(client_id='p', clean_session=False)
client.connect(host='10.1.128.166', port=1883)
client.subscribe(topic, qos=2)
client.subscribe(topic1, qos=2)
client.subscribe(topic2, qos=2)


#Pub
broker_address = "10.1.128.166"

qtCreatorFile = "domoticaUI.ui" # Nombre del archivo aquí.
Ui_MainWindow, QtBaseClass = uic.loadUiType(qtCreatorFile)

class MyApp(QtWidgets.QMainWindow, Ui_MainWindow):

    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)   
        Ui_MainWindow.__init__(self)
        self.setupUi(self)
        self.sensor1.clicked.connect(self.Temperatura)
        self.sensor2.clicked.connect(self.Alarma)
        self.sensor3.clicked.connect(self.Luminocidad)
        
        self.sensor1_1.clicked.connect(self.Temperatura_1)
        self.sensor2_1.clicked.connect(self.Alarma_1)
        self.sensor3_1.clicked.connect(self.Luminocidad_1)
        
        self.temperatura.setText(' Desactivado')
        self.alarma.setText(' Desactivado')
        self.estadoActual.setText(' Desactivado')
        
        self.sensor1.setCheckable(True)
        self.sensor2.setCheckable(True)
        self.sensor3.setCheckable(True)
        
        

    def Temperatura(self):
        global bandera1
        if bandera1 == 0:

            publish.single(topic3, "1", hostname=broker_address,client_id='p')
            bandera1 = 1
            self.temperatura.setText(' Activado')
            
        else:
            publish.single(topic3, "2", hostname=broker_address,client_id='p')
            bandera1 = 0
            self.temperatura.setText(' Desactivado')
            
    def Temperatura_1(self):
        
        def Pump_callback(client, userdata, message):
            a=str(message.payload)
            self.temperatura_2.setText(a)
        client.message_callback_add(topic,Pump_callback)
        
        for x in range (0,4):
            client.loop_read()
            time.sleep(0.1)


    def Alarma(self):
        global bandera2
        if bandera2 == 0:

            publish.single(topic3, "3", hostname=broker_address,client_id='p')
            bandera2 = 1
            self.alarma.setText(' Activado')
            
        else:
            publish.single(topic3, "4", hostname=broker_address,client_id='p')
            bandera2 = 0
            self.alarma.setText(' Desactivado')
            
    def Alarma_1(self):
        def Pump_callback1(client, userdata, message):
            a=str(message.payload)
            self.alarma_2.setText(a)
        client.message_callback_add(topic1,Pump_callback1)
        
        for x in range (0,4):
            client.loop_read()
            time.sleep(0.1)
        
    def Luminocidad(self):
        global bandera3
        if bandera3 == 0:

            publish.single(topic4, "1", hostname=broker_address,client_id='p')
            bandera3 = 1
            self.estadoActual.setText(' Activado')
        else:
            publish.single(topic4, "0", hostname=broker_address,client_id='p')
            bandera3 = 0
            self.estadoActual.setText(' Desactivado')
            
    def Luminocidad_1(self):
        def Pump_callback2(client, userdata, message):
            a=str(message.payload)
            self.estadoActual_2.setText(a)
        client.message_callback_add(topic2,Pump_callback2)
        
        for x in range (0,4):
            client.loop_read()
            time.sleep(0.1)

if __name__ == "__main__":
    app =  QtWidgets.QApplication(sys.argv)
    window = MyApp()
    window.show()
    sys.exit(app.exec_())
