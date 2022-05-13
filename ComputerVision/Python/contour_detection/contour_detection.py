import cv2 as cv
import numpy as np
import os

from typing import List

DEBUG=False

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
        img_lanczos4=cv.resize(img,(width,height),interpolation=cv.INTER_LANCZOS4)
        _,img_thresh=cv.threshold(img_lanczos4,threshold,maxval=255,type=cv.THRESH_BINARY)
        contours,_=cv.findContours(img_thresh,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
        img_rgb=cv.cvtColor(img_thresh,cv.COLOR_GRAY2RGB)
        cv.drawContours(img_rgb,contours,-1,(255,0,0),3)
        for contour in contours:
            M=cv.moments(contour)
            Cx,Cy=int(M['m10']/M['m00']), int(M['m01']/M['m00'])
            cv.circle(img_rgb,(Cx,Cy),3,(0,0,255),-1)
        cv.imwrite(file+str(index)+".jpg",img_rgb)
        index+=1

def main():
    filename="sample1.npy"
    if DEBUG: filename=os.getcwd()+"/ComputerVision/Python/contour_detection/"+filename
    to_images(filename)


if __name__ == '__main__':
    main()