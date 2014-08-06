__author__ = 'nils'

import cv2
import numpy as np

CLASS_UP = 1
CLASS_DOWN = 2
CLASS_STEADY = 0
CLASS_RIGHT = 3


class Classifier:

    def __init__(self):
        self.data = np.array([])
        self.classes = np.array([])
        self.knn = cv2.KNearest()

    def train(self):
        self.load_pose("up", CLASS_UP)
        self.load_pose("steady", CLASS_STEADY)
        self.load_pose("down", CLASS_DOWN)
        self.load_pose("right", CLASS_RIGHT)

        self.knn.train(self.data, self.classes)

    def load_pose(self, name, class_int):
        data_tmp = np.loadtxt("../driver/pose_data/" + name + ".dat").astype(np.float32)
        classes_tmp = np.full(len(data_tmp), class_int).astype(np.float32)
        if len(self.data) == 0:
            self.data = data_tmp
            self.classes = classes_tmp
        else:
            self.data = np.concatenate((self.data, data_tmp))
            self.classes = np.concatenate((self.classes, classes_tmp))

    def classify(self, data):
        undef, results, undef, undef = self.knn.find_nearest(data, 10)
        return results