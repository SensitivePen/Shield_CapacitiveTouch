import serial
import threading
import time

from collections import deque
from typing import List

_lock = threading.Lock()

class SerialThread(threading.Thread):
    def __init__(self,port:str,baudrate:int,buffer_size:int=1)->None:
        super().__init__()
        self.port=port
        self.baudrate=baudrate
        self.flag=False
        self.data=""
        try:
            self.sr=serial.Serial(port,baudrate)
            self.flag=True
        except Exception as e:
            print(e)
        if not self.sr.isOpen() & self.flag:
            self.sr.open()
        self.last_ready = False
        self.buffer=deque(maxlen=buffer_size)
        self._stop=threading.Event()
    
    def run(self)->None:
        if(self.flag):
            while True:
                if self._stop.is_set():
                    break
                if (self.sr.inWaiting()==0):
                    time.sleep(0.001)
                    continue
                self.data=self.sr.readline().decode('utf-8')
                _values=self.data[:-2].split(",")
                #print("debug: ")
                self.buffer.append(_values)
                # print(self.buffer)
                self.last_ready=True
            self.sr.close()
    
    def get_last_values(self)->List[str]:
        """Return the last element of the serial stream"""
        with _lock:
            if not self.last_ready:
                return None
            _last=self.buffer[-1]
            self.last_ready=False
            return _last
    
    def stop(self):
        self._stop.set()

