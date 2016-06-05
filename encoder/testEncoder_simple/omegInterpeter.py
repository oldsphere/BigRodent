# -*- coding: utf-8 -*-

import re
import serial.tools.list_ports as list_ports
from datetime import datetime
import matplotlib.pyplot as plt

 

def timeElapsed(timeVector):
    return [ (time - timeVector[0]).seconds for time in timeVector]
        
    
def plotData(saveAs=None):
    dates = timeElapsed(timeList)
    plt.plot(dates,omegaList)
    plt.grid(True)
    plt.ylabel("Omega [rad/s]")
    plt.xlabel("Time [s]")
    
    if saveAs:
        fil = open(saveAs,"w+")
        textPlot = ["import matplotlib.pyplot as plt",
                    "omega={0}".format(omegaList),
                    "dates={0}".format(dates),
                    "plt.plot(dates,omega)",
                    "plt.grid(True)",
                    "plt.ylabel(\"Omega [rad/s]\")",
                    "plt.xlabel(\"Time [s]\")"]
        print("\n".join(textPlot),file=fil)


ardPort = ""

ports = list(serial.tools.list_ports.comports())
for portName in ports[1]:
    if portName.startswith('Arduino'):
        ardPort = re.search(r"COM\d+",portName).group()
        print(ardPort)
   
try:    
    fil = open('log.txt','a')
    ard = serial.Serial(ardPort,baudrate=9600)        
    print("Machine time:\t\tArdino Time:")
    print("Machine time:\tArdino Time:",file=fil)
    
    # Initialice the variables.
    omeg = {'1':[],'2':[],'3':[],'4':[],'5':[],'6':[],'7':[]}
    pulses = {'1':[],'2':[],'3':[] }
    sTime = {'1':[],'2':[],'3':[] }
    timeList = list()
    clock = list()

    # Start the read loop
    while True:
        ardLine = ard.readline();
        ardLine = ardLine.decode('utf-8').replace('\n','').replace('\r','')
        localTime = datetime.now().strftime("%d/%m/%Y %H:%M:%S")
        print("{0}\t{1}".format(localTime,ardLine))
        
        # Catch the date (if possible)67
        try:
            resTime = re.search("\d+/\d+/\d+\s+\d+:\d+:\d+",ardLine)
            cTime = datetime.strptime(resTime.group(), "%d/%m/%Y %H:%M:%S")
            timeList.append(cTime)
        except:
            pass
        
        # Catch the omega
        try:
            omegResult = re.findall(r"Omega(\d).(\d+\.?\d*)",ardLine)
            for elem in  omegResult:
                omeg[elem[0]].append(float(elem[1]))
        except:
            pass
        
        # Catch the number of pulses
        try:
            pulsesResult = re.findall(r"nPulses(\d).(\d+\.?\d*)",ardLine)
            for elem in  pulsesResult:
                pulses[elem[0]].append(float(elem[1]))
        except:
            pass
        
        # Catch sTime 
        try:
            sTimeResult = re.findall(r"sTime_(\d).(\d+\.?\d*)",ardLine)
            for elem in  sTimeResult:
                sTime[elem[0]].append(float(elem[1]))
        except:
            pass    
        
        try:
            clockResults = re.search(r"clock:(\d+\.?.\d*)",ardline)
            clock = clock.append(clockResults.group(1))
        except:
            pass

except serial.SerialException:
    if ardPort == "":
        print("No available port detected")
        print("No available port detected\n\n",file=fil)
        
    else:
        print("Error opening the port")
        print("Error opening the port\n\n",file=fil)
except KeyboardInterrupt:
    print("Closing connection...")    
    print("Closing connection...\n\n",file=fil)    
    
    #plotData(saveAs="test-fig.py")
 
ard.close()
fil.close()

 