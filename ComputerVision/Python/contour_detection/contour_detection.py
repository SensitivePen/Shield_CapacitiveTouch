import cv2 as cv
import numpy as np
import os

from typing import List

DEBUG=True

NUM_TX=4
NUM_RX=6

min_value, max_value = 0, 200
width, height = NUM_RX*100, NUM_TX*100

t=0.55
threshold = t*255 

img_format=".jpg"
filename = 'sample'
IN_TXT = False

if IN_TXT: filename_input=filename+'.txt'
else: filename_input=filename+'.npy'
if DEBUG: filename_input=os.getcwd()+"/ComputerVision/Python/contour_detection/"+filename_input

filename_output="image/"

def to_images(file_i:str=filename_input,file_o:str=filename_output)->None:
    index=1
    if IN_TXT: data=np.loadtxt(file_i,dtype='int32',delimiter=',')
    else: data=np.load(file_i)
    for line in data:
        # clean data
        line[line > max_value] = max_value
        line[line < min_value] = min_value
        line[True]=np.around((line-min_value)/(max_value-min_value)*255)
        # data to image
        img=line.astype(dtype='uint8').reshape((NUM_TX,NUM_RX))
        img_lanczos4=cv.resize(img,(width,height),interpolation=cv.INTER_LANCZOS4)
        _,img_thresh=cv.threshold(img,threshold,maxval=255,type=cv.THRESH_BINARY)
        contours,_=cv.findContours(img_thresh,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
        cv.imwrite(file_o+str(index)+img_format,img_lanczos4)
        index+=1

def detect_contour(file_i:str,file_o:str):
        img=cv.imread(file_i)
        _,img_thresh=cv.threshold(img,threshold,maxval=255,type=cv.THRESH_BINARY)
        contours,_=cv.findContours(img_thresh,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
        img_rgb=cv.cvtColor(img_thresh,cv.COLOR_GRAY2RGB)
        cv.drawContours(img_rgb,contours,-1,(255,0,0),3)
        for contour in contours:
            M=cv.moments(contour)
            Cx,Cy=int(M['m10']/M['m00']), int(M['m01']/M['m00'])
            cv.circle(img_rgb,(Cx,Cy),3,(0,0,255),-1)
        cv.imwrite(file_o+img_format)

def main():
    to_images()


if __name__ == '__main__':
    main()