#!/usr/bin/python

__author__ = 'nils'

import numpy as np
import PlotData
from USB import USB

if __name__ == '__main__':

    # Initialize the device
    usbDevice = USB()

    # Calibrate
    print "==> Calibrating.."
    #calibrateData = np.mean(np.array([usbDevice.catch_data() for i in xrange(500)]), axis=0)
    #print calibrateData
    print "==> ..ended"
    #usbDevice.offset_theta, usbDevice.offset_phi = calibrateData[1:3]

    # Plot
    analogData = PlotData.AnalogData(100)
    analogPlot = PlotData.AnalogPlot(analogData)


    while True:
        try:
            analogData.add(usbDevice.catch_data())
            analogPlot.update(analogData)

        except KeyboardInterrupt:
                print('exiting')
                break
