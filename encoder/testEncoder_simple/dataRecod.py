"""
- dataRecord.py -
Author: Carlos Rubio Abujas <oldsphere@gmail.com>
Date: 20/02/2016
Description:
    Get the measures from the arduino and stores them in a csv file.
"""

import re
from serial import Serial
import serial.tools.list_ports as list_ports
from datetime import datetime
import pandas as pd

def now():
    """ Returns current time """
    return datetime.strftime(datetime.now(),'%d-%m-%Y %H:%M:%S')
  
def byte2str(line):
    """ convert the read line from bytes to str """
    if type(line) == str: return line
    return line.decode('utf-8',errors='ignore'). \
           replace('\n','').replace('\r','')
    
def scanPorts(portName=None, quiet=False):
    """ 
    This function scan all the available Serial ports and returns the selected
    one if possible.
    """
    ports = list( list_ports.comports() )
    userPort = None
    if not quiet:
        print("Listing serial ports...")
    for port in ports:
        if not quiet:
            print(port[0],":",port[1])
        
        if portName:
            if portName.upper() in port[1].upper():
                userPort = port[0]
    return userPort

def getData(line):
    """
        Return the required information from the line if possible
    """
    try:    
        pattern = r'.*time=(\d+\.?\d*).*omega=(\d+\.?\d*).+nPulses=(\d+\.?\d*)'
        measure = re.findall(pattern,line)[0]
        return {'time': measure[0], 
                'omega': measure[1],
                'pulses':measure[2]}
    except IndexError:
        return None

def saveData(dataRecorded,fileName="default.csv",**kwargs):
    if len(dataRecorded) == 0:
        print("No data recorded")
        return
    df = pd.DataFrame(dataRecorded)
    df = df.convert_objects(convert_numeric=True)
    df.to_csv(fileName,**kwargs)
    print("Data have been saved")
        
        
        
# -----------------------------------------------------------------------------
        
recordFile = 'test-01.csv'        
port = scanPorts(portName='USB Serial', quiet=True)
dataRecord = list()
with Serial(port,9600) as ard:
    while True:
        try:
            cLine = byte2str(ard.readline())
            print('{0}  >  {1}'.format(now(),cLine),end="")
            data = getData(cLine)
            if data: 
                dataRecord.append(data)
                print("  [RECORDED]")
            else:
                print("")
        except KeyboardInterrupt:
            print("Interrupted by user")
            saveData(dataRecord,fileName=recordFile)
            break
        
            
            
                 
        
        