__author__ = 'nils'

from collections import deque

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np


# class that holds analog data for N samples
class AnalogData:
    # constr
    def __init__(self, maximum_length):
        self.ax = deque([0.0]*maximum_length)
        self.ay = deque([0.0]*maximum_length)
        self.az = deque([0.0]*maximum_length)

        self.current_x = [0, 1]
        self.current_y = [0, 1]
        self.current_z = [0, 1]
        self.maxLen = maximum_length

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

        self.current_x = [0, self.ax[0]]
        self.current_y = [0, self.ay[0]]
        self.current_z = [0, self.az[0]]


# plot class
class AnalogPlot:
    def __init__(self, analog_data):
        # set plot to animated
        self.limit = 0.5

        plt.ion()
        fig = plt.figure()
        self.ax = fig.gca(projection='3d')

        self.arrow, = self.ax.plot([0], [0], [0])
        self.contour_x, = self.ax.plot([0], [0], color="black")
        self.contour_y, = self.ax.plot([0], [0], color="black")
        self.contour_z, = self.ax.plot([0], [0], color="black")

        self.last_x, = self.ax.plot([0], [0], color="gray")
        self.last_y, = self.ax.plot([0], [0], color="gray")
        self.last_z, = self.ax.plot([0], [0], color="gray")

        self.ax.set_xlim(-self.limit, self.limit)
        self.ax.set_ylim(-self.limit, self.limit)
        self.ax.set_zlim(-self.limit, self.limit)

        phi = np.linspace(- np.pi, np.pi)
        x = self.limit*np.sin(phi)
        y = self.limit*np.cos(phi)

        self.ax.plot(x, y, zs=-self.limit, zdir="x", color="black")
        self.ax.plot(x, y, zs=self.limit, zdir="y", color="black")
        self.ax.plot(x, y, zs=-self.limit, zdir="z", color="black")

    # update plot
    def update(self, analogData):
        self.arrow.set_xdata(analogData.current_x)
        self.arrow.set_ydata(analogData.current_y)
        self.arrow.set_3d_properties(analogData.current_z)

        self.contour_x.set_xdata(analogData.current_y)
        self.contour_x.set_ydata(analogData.current_z)
        self.contour_x.set_3d_properties(zs=-self.limit, zdir="x")

        self.contour_y.set_xdata(analogData.current_x)
        self.contour_y.set_ydata(analogData.current_z)
        self.contour_y.set_3d_properties(zs=self.limit, zdir="y")

        self.contour_z.set_xdata(analogData.current_x)
        self.contour_z.set_ydata(analogData.current_y)
        self.contour_z.set_3d_properties(zs=-self.limit, zdir="z")

        self.last_x.set_xdata(analogData.ay)
        self.last_x.set_ydata(analogData.az)
        self.last_x.set_3d_properties(zs=-self.limit, zdir="x")

        self.last_y.set_xdata(analogData.ax)
        self.last_y.set_ydata(analogData.az)
        self.last_y.set_3d_properties(zs=self.limit, zdir="y")

        self.last_z.set_xdata(analogData.ax)
        self.last_z.set_ydata(analogData.ay)
        self.last_z.set_3d_properties(zs=-self.limit, zdir="z")
        plt.draw()
