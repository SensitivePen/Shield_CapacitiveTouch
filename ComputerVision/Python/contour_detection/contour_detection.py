import cv2 as cv
import numpy as np
import os

from typing import List

NUM_TX=4
NUM_RX=6

min_value, max_value = 8, 150
width, height = NUM_RX*100, NUM_TX*100

t=0.55
threshold = t*255 

img_format=".jpg"


def read_data(filename:str)->np.ndarray:
    return np.load(filename)

def clean_data(data:np.ndarray)->None:
    for line in data:
        line[line > max_value] = max_value
        line[line < min_value] = min_value

def map_data(data:np.ndarray)->None:
    for line in data:
        line[True]=(line-min_value)/(max_value-min_value)

def to_images(filename:str):
    index=1
    data=np.load(filename)
    file="image/"
    for line in data:
        # clean data
        line[line > max_value] = max_value
        line[line < min_value] = min_value
        line[True]=np.around((line-min_value)/(max_value-min_value)*255)
        # data to image
        img=line.astype(dtype='uint8').reshape((NUM_TX,NUM_RX))
        img=cv.resize(img,(600,400),interpolation=cv.INTER_LANCZOS4)
        # binary image
        img[img<threshold]=0
        img[img>threshold]=255
        cv.imwrite(file+str(index)+".jpg",img)
        index+=1

def main():
    filename2="sample1.npy"
    to_images(filename2)


if __name__ == '__main__':
    main()