import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import time

from drawnow import *
from serial_thread import SerialThread

NUM_TX=4
NUM_RX=6

MIN=10
MAX=100
matrix=np.zeros((NUM_TX,NUM_RX))

port="COM6"
baudrate=115200

def _toArray(data)->None:
    for tx in range(NUM_TX):
        for rx in range(NUM_RX):
            matrix[tx][rx]=int(data[tx*NUM_RX+rx])

def plotMatrix()->None:
    plt.imshow(matrix,interpolation='nearest',cmap='inferno',origin='lower',vmin=MIN,vmax=MAX)
    plt.colorbar() 
    plt.show()

def main()->None:
    serial_thread=SerialThread(port=port,baudrate=baudrate,buffer_size=2)
    serial_thread.start()
    plt.ion() #Tell matplotlib you want interactive mode to plot live data
    while True:
        data=serial_thread.get_last_values()
        if data is None:
            time.sleep(0.001)
            continue
        _toArray(data)
        drawnow(plotMatrix)
        plt.pause(0.0001)
        if len(plt.get_fignums())==0:
            break
    plt.close()
    serial_thread.stop()
            
if __name__ == '__main__':
    main()
