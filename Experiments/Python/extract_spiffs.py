from fileinput import filename
import glob
import os
import sys
import serial

from typing import List

baudrate=115200

def serial_ports()->List[str]:
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

def select_port()->str:
    ports = serial_ports()
    if(len(ports)>0):
        for i in range(len(ports)):
            print(f"{i}: {ports[i]}")
        index=input("Select your port: ")
        os.system('cls' if os.name == 'nt' else 'clear')
        return ports[int(index)]
    print("No ports available")
    return None

def extract(path:str, serial_port:str, baudrate:int=112500,timeout:float=1., format:str=".csv")->None:
    movuino = serial.Serial(serial_port, baudrate=baudrate,timeout=timeout)
    file_index = 1
    datafile=""
    
    # Send to the Movuino the 'read SPIFF' instruction
    movuino.write(bytes("s",'ascii'))

    while True:
        line = movuino.readline().decode('utf-8')
        if("XXX_beginning" in line):
            print("Record begins")
            continue
        if("XXX_newRecord" in line or "XXX_end" in line):
            if datafile!="":
                with open(path+str(file_index)+format,'w') as file:
                    file.write(datafile)
            if("XXX_end" in line):
                break
            datafile=""
            file_index+=1
            continue
        if line!='':
            datafile+=line.strip()+'\n'
    movuino.close()

def main():
    port = select_port()
    dir="gain_100/"
    filename=""
    path=dir+filename
    if not os.path.exists(dir):
        os.makedirs(dir)
    if port is not None:
        extract(path,port)

if __name__=='__main__':
    main()



