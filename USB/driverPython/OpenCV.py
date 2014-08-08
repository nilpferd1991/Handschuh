__author__ = 'nils'

import cv2
import numpy as np

CLASS_UP = 1
CLASS_DOWN = 2
CLASS_STEADY = 0
CLASS_RIGHT = 3
CLASS_NONE = -1


class Classifier:

    def __init__(self):
        self.data = np.array([])
        self.classes = np.array([])

    def train_pose(self):
        self.load_pose("up", CLASS_UP)
        self.load_pose("steady", CLASS_STEADY)
        self.load_pose("down", CLASS_DOWN)
        self.load_pose("right", CLASS_RIGHT)

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
        pass


class MLPClassifier(Classifier):
    def __init__(self):
        Classifier.__init__(self)
        self.output_len = 4
        self.input_len = 3
        self.hidden_len = 5
        self.classifier = cv2.ANN_MLP(np.array([self.input_len, self.hidden_len, self.output_len]))

    def train_pose(self):
        Classifier.train_pose(self)
        params = dict(term_crit=(cv2.TERM_CRITERIA_COUNT | cv2.TERM_CRITERIA_EPS, 10000, 0.0001),
                      train_method = cv2.ANN_MLP_TRAIN_PARAMS_BACKPROP,
                      bp_dw_scale = 0.01,
                      bp_momentum_scale = 0.0)

        classes_tmp = np.zeros((len(self.data), self.output_len))
        for i in xrange(len(self.classes)):
            classes_tmp[i, self.classes[i]] = 1
        self.classifier.train(self.data, classes_tmp, None, params=params)

    def classify(self, data):
        ret, output = self.classifier.predict(data)
        if np.amax(output) > 0.8:
            return np.argmax(output)
        else:
            return CLASS_NONE

class KNClassifier(Classifier):
    def __init__(self):
        Classifier.__init__(self)
        self.classifier = cv2.KNearest()
        self.neighbors = 10

    def train_pose(self):
        Classifier.train_pose(self)
        self.classifier.train(self.data, self.classes)

    def classify(self, data):
        ret, undef, neighbors, distances = self.classifier.find_nearest(data, self.neighbors)
        if np.mean(distances) < 0.05:
            return ret
        else:
            return CLASS_NONE