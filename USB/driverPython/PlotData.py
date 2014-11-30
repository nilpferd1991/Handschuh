__author__ = 'nils'

from collections import deque

import matplotlib.pyplot as plt


# class that holds analog data for N samples
class AnalogData:
    # constr
    def __init__(self, maxLen):
        self.ax = deque([0.0]*maxLen)
        self.ay = deque([0.0]*maxLen)
        self.az = deque([0.0]*maxLen)
        self.maxLen = maxLen

    # ring buffer
    def add_to_buffer(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.pop()
            buf.appendleft(val)

    # add data
    def add(self, data):
        self.add_to_buffer(self.ax, data[0])
        self.add_to_buffer(self.ay, data[1])
        self.add_to_buffer(self.az, data[2])


# plot class
class AnalogPlot:
    def __init__(self, analog_data):
        # set plot to animated
        plt.ion()
        self.axline, = plt.plot(analog_data.ax, label="r")
        self.ayline, = plt.plot(analog_data.ay, label="theta")
        self.azline, = plt.plot(analog_data.az, label="phi")
        plt.ylim([-4, 4])

    # update plot
    def update(self, analogData):
        self.axline.set_ydata(analogData.ax)
        self.ayline.set_ydata(analogData.ay)
        self.azline.set_ydata(analogData.az)
        plt.legend()
        plt.draw()
