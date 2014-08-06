#!/usr/bin/python

import usb.core
import ctypes
import numpy as np
from matplotlib import pyplot as plt
import subprocess
import OpenCV

#########################################################################
# Constants (aus certificates.h)
BUFFER_SIZE = 8

CUSTOM_RQ_DATA = 1
CUSTOM_RQ_DATA_LEN = BUFFER_SIZE

CUSTOM_RQ_LOG = 2
CUSTOM_RQ_LOG_LEN = BUFFER_SIZE

CUSTOM_RQ_TOGGLE = 3
CUSTOM_RQ_TOGGLE_LEN = 0


#########################################################################
class USB:
    def __init__(self):

        self.offset_r = 0
        self.offset_theta = 0
        self.offset_phi = 0

        # Search device (Vendor- and Product-ID)
        self.device = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)

        # Found?
        if self.device is None:
            raise ValueError('Device not found')

        # set the active configuration. With no arguments, the first
        # configuration will be the active one
        self.device.set_configuration()

    def catch_data(self):
        sensor_data = self.device.ctrl_transfer(0xC0, CUSTOM_RQ_DATA, 0, 0, CUSTOM_RQ_DATA_LEN)

        x = ctypes.c_int8(sensor_data[1]).value << 2 | ctypes.c_int8(sensor_data[0] >> 6).value
        y = ctypes.c_int8(sensor_data[3]).value << 2 | ctypes.c_int8(sensor_data[2] >> 6).value
        z = ctypes.c_int8(sensor_data[5]).value << 2 | ctypes.c_int8(sensor_data[4] >> 6).value

        return self.map(np.array([z/512.0, x/512.0, y/512.0]))

    @staticmethod
    def map(coords):
        return np.array([np.sqrt(coords[0]**2 + coords[1]**2 + coords[2]**2),
                         np.arctan2(coords[2], np.sqrt(coords[0]**2 + coords[1]**2)),
                         np.arctan2(coords[0], coords[2])])

    def catch_calibrated_data(self):
        return np.array([self.catch_data() - np.array([self.offset_r, self.offset_theta, self.offset_phi])])\
            .astype(np.float32)

    def catch_log(self):
        return self.device.ctrl_transfer(0xC0, CUSTOM_RQ_LOG, 0, 0, CUSTOM_RQ_LOG_LEN)

    def toggle(self):
        return self.device.ctrl_transfer(0xC0, CUSTOM_RQ_TOGGLE, 0, 0, CUSTOM_RQ_TOGGLE_LEN)


##########################################################################
# class that holds analog data for N samples
class AnalogData:
    # constructor
    def __init__(self, max_len):
        self.ax = np.zeros(max_len)
        self.ay = np.zeros(max_len)
        self.az = np.zeros(max_len)
        self.max_len = max_len

    # ring buffer
    def add_to_buf(self, buf, val):
        if len(buf) < self.max_len:
            buf.insert(val)
        else:
            buf = np.roll(buf, 1)
            buf[-1] = val

    # add data
    def add(self, data):
        assert(len(data) == 3)
        self.add_to_buf(self.ax, data[0])
        self.add_to_buf(self.ay, data[1])
        self.add_to_buf(self.az, data[2])


# plot class
class AnalogPlot:
        # constructor
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


def check_buffer(status_buffer):
    if len(status_buffer) > 0 and status_buffer[-1] == 0.0:
        if status_buffer[-3:] == [0, 2, 0]:
            subprocess.call("sudo -u nils dbus-send --session --type='method_call' --dest='org.bansheeproject.Banshee'"
                            " '/org/bansheeproject/Banshee/PlaybackController'"
                            " 'org.bansheeproject.Banshee.PlaybackController.Next' 'boolean:false'", shell=True)
            print "up"
        elif status_buffer[-3:] == [0, 1, 0]:
            subprocess.call("sudo -u nils dbus-send --session --type='method_call' --dest='org.bansheeproject.Banshee'"
                            " '/org/bansheeproject/Banshee/PlaybackController'"
                            " 'org.bansheeproject.Banshee.PlaybackController.Previous' 'boolean:false'", shell=True)
            print "down"
        elif status_buffer[-3:] == [0, 3, 0] or status_buffer[-3:] == [3, 2, 0]:
            subprocess.call("sudo -u nils dbus-send --session --type='method_call' --dest='org.bansheeproject.Banshee'"
                            " '/org/bansheeproject/Banshee/PlayerEngine'"
                            " 'org.bansheeproject.Banshee.PlayerEngine.Pause'", shell=True)
            print "right"
        elif len(status_buffer) > 1:
            print status_buffer
        return [0.0]
    else:
        return status_buffer


if __name__ == '__main__':

    # Initialize the device
    usbDevice = USB()
    file = open("output.dat", "w")

    # Initialize the classifier
    classifier = OpenCV.Classifier()
    classifier.train()

    # Calibrate
    print "==> Calibrating.."
    calibrateData = np.mean(np.array([usbDevice.catch_data() for i in xrange(500)]), axis=0)
    print calibrateData
    print "==> ..ended"
    usbDevice.offset_theta, usbDevice.offset_phi = calibrateData[1:3]

    status = 0
    counter = 0
    tmpStatus = -1
    statusBuffer = list()

    while True:
        try:
            data = usbDevice.catch_calibrated_data()
            newStatus = classifier.classify(data)[0, 0]
            if newStatus != tmpStatus:
                counter = 1
                tmpStatus = newStatus

            if counter > 0:
                counter += 1

            if counter > 15:
                counter = 0
                status = tmpStatus
                statusBuffer.append(status)

            statusBuffer = check_buffer(statusBuffer)

            #file.write(str(data[0, 0]) + "\t" + str(data[0, 1]) + "\t" + str(data[0, 2]) + "\t" + str(tmpStatus) +
            #           "\t" + str(status) + "\n")


        except KeyboardInterrupt:
                print('exiting')
                file.close()
                break
