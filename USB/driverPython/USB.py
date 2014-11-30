__author__ = 'nils'

import usb.core
import ctypes
import numpy as np

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
        self.device = None

        self.search_for_usb_device()

    def search_for_usb_device(self):
        # Search device (Vendor- and Product-ID)
        self.device = usb.core.find(idVendor=0x16c0, idProduct=0x05dc)
        # Found?
        if self.device is None:
            raise ValueError('Device not found')

        # set the active configuration. With no arguments, the first
        # configuration will be the active one
        self.device.set_configuration()

    def catch_raw_data(self):
        sensor_data = self.device.ctrl_transfer(0xC0, CUSTOM_RQ_DATA, 0, 0, CUSTOM_RQ_DATA_LEN)

        x = ctypes.c_int16((sensor_data[1] << 8) + sensor_data[0]).value
        y = ctypes.c_int16((sensor_data[3] << 8) + sensor_data[2]).value
        z = ctypes.c_int16((sensor_data[5] << 8) + sensor_data[4]).value
        count = (sensor_data[7] << 8) + sensor_data[6];

        return np.array([-y/512.0/count, x/512.0/count, z/512.0/count])

    def catch_data(self):
        return self.map(self.catch_raw_data())

    @staticmethod
    def map(coords):
        return np.array([np.sqrt(coords[0]**2 + coords[1]**2 + coords[2]**2),
                         np.arccos(coords[2]/np.sqrt(coords[0]**2 + coords[1]**2 + coords[2]**2)),
                         np.arctan2(coords[1], coords[0])])

    def catch_calibrated_data(self):
        return np.array(self.catch_data() - np.array([self.offset_r, self.offset_theta, self.offset_phi]))\
            .astype(np.float32)

    def catch_log(self):
        return self.device.ctrl_transfer(0xC0, CUSTOM_RQ_LOG, 0, 0, CUSTOM_RQ_LOG_LEN)

    def toggle(self):
        return self.device.ctrl_transfer(0xC0, CUSTOM_RQ_TOGGLE, 0, 0, CUSTOM_RQ_TOGGLE_LEN)
