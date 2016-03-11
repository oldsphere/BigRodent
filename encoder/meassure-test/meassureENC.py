'''
    - measureENC.py -
    Author: Carlos Rubio Abujas <oldsphere@gmail.com>
    Date: 08/03/2016
    Description:
        Decode de information contained in a svg file.
        The svg file must contain a specific format, considering the following
        convention:
        
            Blue lines are interpreted as reference (color: #0000ff)
            Red lines are interpreted as scale (color: #ff0000) [UNUSED]
            Green lines are interpreted as meassures (color #00ff00)
            
        Blue lines will be employeed to locate the center of the encoder. This
        center will be used to determine the angle of any of the points 
        meassured on the green points.
        
        For the code to work properly there must be twice refernce lines for 
        every meassure line. Meassure lines may have more than a sinle point
 '''

import pySVG
import numpy as np
from math import *

# color code:
scaleColor = '#ff0000'      # Red
lineColor = '#00ff00'       # Green
refLineColor = '#0000ff'    #Blue

# Open svg file
fileName = '../ENC01/ENC01.svg'
svg = pySVG.svg(fileName)

# Get lines and properties 
#(Choose only paths contained in layers directly dependent of the svg tag)
svgPaths = svg.get_elementByType('//svg:svg/svg:g/svg:path')
lines = []
for path in svgPaths:
    pathAttributes = svg.get_attrib(path)
    
    lines.append( {'element':path,
                   'points': pySVG.get_pathPoints( pathAttributes['d'] ),
                   'id': pathAttributes['id'],
                   'style' : pathAttributes['style']} )
                   
scale = [ line for line in lines if 'stroke:'+scaleColor in line['style'] ]
meassures = [ line for line in lines if 'stroke:'+lineColor in line['style'] ]
angleReferences = [line for line in lines if 'stroke:'+refLineColor in line['style'] ]
                                       
# Sort the meassurements by decreasing x0 position.
meassures.sort(key=lambda x: x['points'][0]['x'], reverse=True)
angleReferences.sort(key=lambda x: x['points'][0]['x'], reverse=True)

def getAngle_legacy(line):
    '''
        Get the angle of a given line. The angle is given using the vertical
        as refernce.
        
        Note: It only considers the position of the first and last point ingoniring 
        any curvature or additional point in the middle.
    '''
    if len(line['points']) < 2: raise AttributeError('Invalid line')
    
    
    # Take the lowest point as the reference    
    p0 = line['points'][0]
    pf = line['points'][-1]    
    if p0['y'] > pf['y']: 
        p0 = line['points'][-1]
        pf = line['points'][0]            
    
    # Normalize the vector.
    npf = { 'x' : pf['x'] - p0['x'],
            'y' : pf['y'] - p0['y']}
    
    if npf['y'] == 0: return 90
    return atan( npf['x'] / npf['y'] ) * 180/pi



def getAngle(p1,p2):
    '''
        
    '''
    dx = p1['x']-p2['x']
    dy = p1['y']-p2['y']
    return atan( dx/dy ) * 180/pi

def getCenter(line1,line2):
    '''
        Return the center point from two straing lines
    '''
    
    x0_1,y0_1 = line1['points'][0]['x'], line1['points'][0]['y']   
    xf_1,yf_1 = line1['points'][-1]['x'], line1['points'][-1]['y']   
    x0_2,y0_2 = line2['points'][0]['x'], line2['points'][0]['y']   
    xf_2,yf_2 = line2['points'][-1]['x'], line2['points'][-1]['y']   
    
    
    mat = np.array([[x0_1-xf_1, 0, 1, 0],
                    [0, x0_2-xf_2, 1, 0],
                    [y0_1-yf_1, 0, 0, 1],
                    [0, y0_2-yf_2, 0, 1]])
                    
    coefs = np.array([x0_1,x0_2,y0_1,y0_2])
    result = np.linalg.inv(mat).dot(coefs.T)
    return { 'x': result[2], 'y' : result[3] }



# Meassure the points

if not len(meassures) == len(angleReferences)/2:
    raise AttributeError('Incomplete data, some reference or meassure is missing')

pulses = []
pairReferences = [(l1, l2) for l1,l2 in zip(angleReferences[::2],angleReferences[1::2])]
for ref, line in zip( pairReferences, meassures):

    center = getCenter(ref[0],ref[1])
    sectionAngle = getAngle(ref[1],center) - getAngle(ref[0],center)
    
    angles = []
    for point in line['points']:
        angles.append( getAngle(point,center) )
    pulses += [a2-a1 for a1,a2 in zip(angles[1::],angles[:-1:]) ]
