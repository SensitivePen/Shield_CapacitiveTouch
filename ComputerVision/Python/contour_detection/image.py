import cv2 as cv
import numpy as np

from math import *
from typing import List, Tuple


class Contour:
    def __init__(self, contour: np.ndarray) -> None:
        self.contour = contour
        self.moments = cv.moments(contour)
        self.set_centre()
        self.set_dimension()

    def set_centre(self) -> None:
        Cx = int(self.moments['m10']/self.moments['m00'])
        Cy = int(self.moments['m01']/self.moments['m00'])
        self._center = (Cx, Cy)

    def set_dimension(self) -> None:
        leftmost = tuple(self.contour[self.contour[:, :, 0].argmin()][0])
        rightmost = tuple(self.contour[self.contour[:, :, 0].argmax()][0])
        topmost = tuple(self.contour[self.contour[:, :, 1].argmin()][0])
        bottommost = tuple(self.contour[self.contour[:, :, 1].argmax()][0])
        self.width = round(((leftmost[0]-rightmost[0])**2 +
                            (leftmost[1]-rightmost[1])**2)**(1/2))
        self.height = round(((topmost[0]-bottommost[0])**2 +
                             (topmost[1]-bottommost[1])**2)**(1/2))

    def get_centre(self) -> Tuple[int, int]:
        return self._center

    def get_dimension(self) -> Tuple[int, int]:
        """return: (width, height)"""
        return self.width, self.height


class Image:
    def __init__(self, filename: str, threshold: float, maxval: int = 255) -> None:
        self.filename = filename
        self.image = cv.imread(filename)
        self.threshold = threshold*maxval
        self.maxval = maxval
        self.centres = []
        self.dimensions = []
        self.contours = self.set_contours()

    def set_contours(self) -> None:
        self.contours = []
        img_gray = cv.cvtColor(self.image, cv.COLOR_BGR2GRAY)
        _, img_thresh = cv.threshold(
            img_gray, self.threshold, maxval=self.maxval, type=cv.THRESH_BINARY)
        contours, _ = cv.findContours(
            img_thresh, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        for contour in contours:
            ctn = Contour(contour)
            self.contours.append(ctn)
            self.centres.append(ctn.get_centre())
            self.dimensions.append(ctn.get_dimension())

    def get_centres(self) -> List[Tuple[int, int]]:
        return self.centres
    
    def get_dimensions(self) -> List[Tuple[int, int]]:
        return self.dimensions

    def get_pressure(self, point: Tuple[int, int]) -> int:
        rgb = self.image[point[::-1]]  # invert (x,y)->(y,x)
        return round(sum(rgb)/len(rgb))
