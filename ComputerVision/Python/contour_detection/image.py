import cv2 as cv
import numpy as np

from math import *
from typing import List, Tuple


class Contour:
    """Class for the caracteristic of a blob in an image"""
    def __init__(self, contour: np.ndarray) -> None:
        self.contour = contour
        self._moments = cv.moments(contour)
        self.set_centre()
        self.set_dimension()

    def set_centre(self) -> None:
        Cx = int(self._moments['m10']/self._moments['m00'])
        Cy = int(self._moments['m01']/self._moments['m00'])
        self._center = (Cx, Cy)

    def set_dimension(self) -> None:
        leftmost = tuple(self.contour[self.contour[:, :, 0].argmin()][0])
        rightmost = tuple(self.contour[self.contour[:, :, 0].argmax()][0])
        topmost = tuple(self.contour[self.contour[:, :, 1].argmin()][0])
        bottommost = tuple(self.contour[self.contour[:, :, 1].argmax()][0])
        self._width = round(((leftmost[0]-rightmost[0])**2 +
                            (leftmost[1]-rightmost[1])**2)**(1/2))
        self._height = round(((topmost[0]-bottommost[0])**2 +
                             (topmost[1]-bottommost[1])**2)**(1/2))

    def get_centre(self) -> Tuple[int, int]:
        """Return the centre of a blob

        Returns:
            Tuple[int, int]: (x,y)
        """
        return self._center

    def get_dimension(self) -> Tuple[int, int]:
        """Return width and height of a blob 

        Returns:
            Tuple[int, int]: (Dx,Dy)
        """
        return self._width, self._height


class Image:
    """Class for detecting the blobs and get info about it"""
    def __init__(self, filename: str, threshold: float, maxval: int = 255) -> None:
        self.filename = filename
        self.image = cv.imread(filename)
        self.threshold = int(threshold*maxval)
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
        """Return the centre of all blobs in the image

        Returns:
            List[Tuple[int, int]]: List of (x,y)
        """
        return self.centres
    
    def get_dimensions(self) -> List[Tuple[int, int]]:
        """Return the width and height of the all blobs in the image

        Returns:
            List[Tuple[int, int]]: List of (Dx,Dy)
        """
        return self.dimensions

    def get_pressure(self, point: Tuple[int, int]) -> int:
        """Return the intensity of the pressure

        Args:
            point (Tuple[int, int]): Point(x,y)

        Returns:
            int: pixel value of a selected point
        """
        rgb = self.image[point[::-1]]  # invert (x,y)->(y,x)
        return round(sum(rgb)/len(rgb))
