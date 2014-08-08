#!/usr/bin/python

__author__ = 'nils'

import numpy as np
from matplotlib import pyplot as plt
import subprocess
import OpenCV
from USB import USB

def check_buffer(status_buffer):

    if len(status_buffer) > 0 and status_buffer[-1][0] == 0:
        return status_buffer
    else:
        return status_buffer

    """
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
        """


if __name__ == '__main__':

    # Initialize the device
    usbDevice = USB()
    file = open("output.dat", "w")

    # Initialize the classifier
    poseClassifier = OpenCV.MLPClassifier()
    poseClassifier.train_pose()

    # Calibrate
    print "==> Calibrating.."
    calibrateData = np.mean(np.array([usbDevice.catch_data() for i in xrange(500)]), axis=0)
    print calibrateData
    print "==> ..ended"
    usbDevice.offset_theta, usbDevice.offset_phi = calibrateData[1:3]

    status = -1
    counter = 0
    statusCounter = 0
    statusBuffer = list()

    while True:
        try:
            newStatus = poseClassifier.classify(usbDevice.catch_calibrated_data())
            print newStatus
            """if newStatus != status and newStatus != -1:
                counter = 1
                status = newStatus

            if counter > 0:
                counter += 1

            statusCounter += 1

            if counter > 25:
                if len(statusBuffer) < 1 or statusBuffer[-1][0] != status:
                    if len(statusBuffer) > 0:
                        statusBuffer[-1][1] = statusCounter
                    statusBuffer.append([status, counter])
                    counter = 0
                    statusCounter = 0
                    if len(statusBuffer) > 1:
                        print statusBuffer[-2]

            statusBuffer = check_buffer(statusBuffer)"""

        except KeyboardInterrupt:
                print('exiting')
                file.close()
                break
