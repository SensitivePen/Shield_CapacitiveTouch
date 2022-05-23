import glob
import numpy as np
import os
import serial
import sys
import time

from serial_thread import SerialThread
from typing import List

baudrate = 115200
filename = 'sample'
recorded_time = 1  # (in seconds)

TO_TXT = False

if TO_TXT: filename=filename+'.txt'
else: filename=filename+'.npy'

def serial_ports() -> List[str]:
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


def select_port() -> str:
    ports = serial_ports()
    if(len(ports) > 0):
        for i in range(len(ports)):
            print(f"{i}: {ports[i]}")
        index = input("Select your port: ")
        os.system('cls' if os.name == 'nt' else 'clear')
        return ports[int(index)]
    print("No ports available")
    return None


def main() -> None:
    port = select_port()
    if port is not None:
        serial_thread = SerialThread(port=port, baudrate=baudrate, buffer_size=2)
        serial_thread.start()
        time.sleep(2)
        t_0 = time.time()
        recorded = None
        while time.time()-t_0 < recorded_time:
            data = serial_thread.get_last_values()
            if data is None:
                time.sleep(0.0001)
                continue
            if recorded is None:
                recorded = data
            else:
                recorded = np.append(recorded, data, axis=0)
        if TO_TXT: np.savetxt(filename,recorded,fmt='%i',delimiter=',')
        else: np.save(filename, recorded)
        serial_thread.stop()

if __name__ == "__main__":
    main()
