#!/usr/bin/python

import usb.core
import ctypes
from collections import deque
from matplotlib import pyplot as plt

#########################################################################
# Konstanten (aus certificates.h)
BUFFER_SIZE = 32

CUSTOM_RQ_ECHO = 0
CUSTOM_RQ_ECHO_LEN = 4

CUSTOM_RQ_DATA = 1
CUSTOM_RQ_DATA_LEN = BUFFER_SIZE

OFFSET_X = 40
OFFSET_Y = 0
OFFSET_Z = -240


#########################################################################
def initUSB():
    # Device suchen (nach Vendor- und Product-ID)
    device = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)
    
    # Gefunden?
    if device is None:
        raise ValueError('Device not found')
    
    # set the active configuration. With no arguments, the first
    # configuration will be the active one
    device.set_configuration()
    
    return device

def catchData(device):
    # requestType: USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN
    # requestCode: 
        
    sensorData = device.ctrl_transfer(0xC0, CUSTOM_RQ_DATA, 0, 0, CUSTOM_RQ_DATA_LEN)
    
    x = ctypes.c_int8(sensorData[1]).value << 2 | ctypes.c_int8(sensorData[0] >> 6).value;
    y = ctypes.c_int8(sensorData[3]).value << 2 | ctypes.c_int8(sensorData[2] >> 6).value;
    z = ctypes.c_int8(sensorData[5]).value << 2 | ctypes.c_int8(sensorData[4] >> 6).value;
    
    return (x - OFFSET_X, y - OFFSET_Y, z - OFFSET_Z)

##########################################################################

def getData(device):
    pos = (0, 0, 0)
    for _ in range(5):
        pos = [a + b for a, b in zip(pos, catchData(device))]
     
    return [x / 5 for x in pos]


##########################################################################
# class that holds analog data for N samples
class AnalogData:
    # constr
    def __init__(self, maxLen):
        self.ax = deque([0.0] * maxLen)
        self.ay = deque([0.0] * maxLen)
        self.az = deque([0.0] * maxLen)
        self.maxLen = maxLen
 
    # ring buffer
    def addToBuf(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.pop()
            buf.appendleft(val)
 
    # add data
    def add(self, data):
        assert(len(data) == 3)
        self.addToBuf(self.ax, data[0])
        self.addToBuf(self.ay, data[1])
        self.addToBuf(self.az, data[2])
        
    
# plot class
class AnalogPlot:
    # constr
    def __init__(self, analogData):
        # set plot to animated
        plt.ion() 
        self.axline, = plt.plot(analogData.ax)
        self.ayline, = plt.plot(analogData.ay)
        self.azline, = plt.plot(analogData.az)
        plt.ylim([-500, 500])
 
    # update plot
    def update(self, analogData):
        self.axline.set_ydata(analogData.ax)
        self.ayline.set_ydata(analogData.ay)
        self.azline.set_ydata(analogData.az)
        plt.draw()

if __name__ == '__main__':
    device = initUSB()
    
    analogData = AnalogData(100)
    analogPlot = AnalogPlot(analogData)
    
    tcounter = 0
    
    while True:
        try:
            data = getData(device)
            analogData.add(data)
            analogPlot.update(analogData)
        except KeyboardInterrupt:
            print 'exiting'
            break
