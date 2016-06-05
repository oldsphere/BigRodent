"""
- dataProcessing.py -
Author: Carlos Rubio Abujas <oldsphere@gmail.com>
Date: 20/02/2016
Description:
    Get the measures from the arduino and stores them in a csv file.
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def plot_omega(data,**kwargs):
    plt.plot(data['time'],data['omega'],**kwargs)
    plt.xlabel('time [s]')
    plt.ylabel('omega [rad/s]')
    plt.grid(True)
    
#data = pd.read_csv('unbalanced-06.csv',comment='"')
data = pd.read_csv('test02-500ms.csv',comment='"')
 
# Add rolling mean column
data['m5_omega'] = pd.rolling_mean(data['omega'],window=5,center=True)
plot_omega(data,marker='o')



    