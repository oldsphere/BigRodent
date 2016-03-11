'''
    - pySVG.py -
    Author: Carlos Rubio Abujas <oldsphere@gmail.com>
    Date: 03/03/2016
    Description:
        usefull funtions to manage SVG files.
        
        Includes a class svg, with browsing options and path functions
'''

import warnings
import re
from lxml import etree
warnings.filterwarnings("ignore")

class svg(object):
    '''
        Include browsing options in the svg structure file
        
        No edition optiosn have been included.
    '''
    def __init__(self,fileName):
        self._fileName = fileName
        self._root = etree.parse(fileName).getroot()
        self._cwd = self._root
        self._NSMAP = {k:v for k,v in self._root.nsmap.items() if k}
        self._elemList = self.get_elementList()
        self._elemIndex = 0

    def translateNS(self, sentence, reverse=False):                
        for ns,key in self._NSMAP.items():
            if not ns: continue
                
            if reverse:
                sentence = sentence.replace('{'+key+'}',ns+':')
            else:
                sentence = sentence.replace(ns+':','{'+key+'}')
        return sentence
    
    def pwd(self):
        print( self.get_path(self._cwd) )
        
    def get_path(self, element):
        path = []
        cw = element
                
        while True:
            path.append ( self.translateNS(cw.tag, reverse=True) )
            cw = cw.getparent()
            if not cw: break
        
        path.reverse()
        return '/ '+' / '.join( path )
        
    def get_element(self, ref):
        if type(ref) == int: 
            return self._elemList[ref]
        if type(ref) == str:
            if ref == '..': return self._cwd.getparent()            
            if ref.count('/') == 0: 
                ref = self.get_path(self._cwd) + '/' + ref
                return self.xpath(ref)
    
    def get_elementList(self, parent=None):
        if not parent: parent = self._cwd
        eList = []
        for element in parent:
            eList.append(element)
        return eList
        
    def ls(self,path=None,cond=None):
        if not path: path = self._cwd        
        else: 
            path = self.get_element(path)
            if type(path) == list: path = path[0]
            
        print( self.get_path(path) )
        for i,element in enumerate( self.get_elementList(path) ):
            if not cond or cond in self.translateNS( element.tag, reverse=True):
                print( str(i) +' -\t' + self.translateNS( element.tag, reverse=True) )
    
    def get_attrib(self, element=None):
        try:   
            if len(element) > 0: element = self._cwd
        except:
            element = self._cwd
        
        attributes = element.attrib
        filtered_attributes = {}
        for key,val in attributes.items():
            filtered_attributes[ self.translateNS(key, reverse=True) ] = val
            
        return filtered_attributes
        
    def get_elementByType(self, elem_type, constraints=None):
        xpath_sentence = '//' + elem_type
        if constraints:
            xpath_sentence += '[' + constraints + ']'
            
        return self.xpath(xpath_sentence)
        
    def get_elementByID(self, elementID):
        return self. get_elementByType( '*',
                                        'contains(@svg:id,' + elementID + ')' )
        
    def home(self):
        self._cwd = self._root
        self._elemList = self.get_elementList()
        
    def cd(self,target):
        target = self.get_element(target)
        if type(target) == list: target = target[0]
        self._cwd = target
        self._elemList = self.get_elementList()
        
    def xpath(self,expression):
        return self._cwd.xpath(expression,namespaces=self._NSMAP)
        
        
def get_pathPoints(path):
    """
        Receive the 'd' parameter of a path in a svg format. This string is 
        analyzed with regex and the points extracted are returned in a vector of
        dictionaties.
        
        The coordinates are given by the inkscape reference.
    """        
    def addPairs(pairList,curve,absolute=False):
        for pointPair in pairList:
            x,y = [float(x) for x in pointPair.split(',')]
            
            try:
                if absolute:
                    points.append( { 'x' : x , 'y' : y } )    
                else:
                    priorPoints = points[-1]
                    points.append( { 'x' : x + priorPoints['x'], 'y' : y + priorPoints['y'] } )                    
            except IndexError:
                points.append( { 'x' : x , 'y' : y } )
        return curve
        
    pointPattern = r"([a-zA-Z]) ((-?\d+\.?\d*,-?\d+\.?\d*\s*)*)"
    curve = re.findall(pointPattern, path)

    points = list()
    for point in curve:
        if point[0] == 'c':
            points = addPairs(point[1].strip().split(' ')[2::3] , points)
            
        if point[0] == 'm' or point[0] == 'l':
            points = addPairs(point[1].strip().split(' ') , points)
            
        if point[0] == 'M' or point[0] == 'L':
            points =  addPairs(point[1].strip().split(' '), points, absolute=True)
                    
    return points
