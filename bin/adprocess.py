#!/usr/bin/env python
#!/bin/env python
# $Id: adprocess.py,v 1.5 2001/07/17 19:28:31 bsmith Exp bsmith $ 
#
# change python to whatever is needed on your system to invoke python
#
#  Processes .c or .f files looking for a particular function.
#  Copies that function as well as an struct definitions 
#  into a new file to be processed with adiC
# 
#  Calling sequence: 
#      adprocess.py file1.[cf] functionname
#
import urllib
import os
import ftplib
import httplib
import re
from exceptions import *
from sys import *
from string import *

#
#  Copies structs from filename to filename.tmp
    
def setupfunctionC(filename):
	newfile = filename + ".tmp"
	f = open(filename)
	g = open(newfile,"w")
        g.write("#include <math.h>\n")
	line = f.readline()
	while line:
                line = lstrip(line)+" "
                reg = re.compile('typedef [ ]*struct')
                fl = reg.search(line)
                if fl:
                        struct = line
			while line:
                                if line[0] == "}":
	 	                	 break
 		                line = f.readline()
                                struct = struct + line
#
#        if this is the AppCtx then replace double and Scalar with passive
#
                        reg = re.compile('[ ]*}[ ]*AppCtx[ ]*;')
                        fl = reg.search(line)
                        if fl:
                                print "Extracting AppCtx structure"
                                reg = re.compile('Scalar ')
                                struct = reg.sub('PassiveScalar ',struct)
                                reg = re.compile('double ')
                                struct = reg.sub('PassiveDouble ',struct)
                        g.write(struct)
		line = f.readline()
	f.close()
        return g

#
#  Appends function functionname from filename to filename.tmp

def getfunctionC(g,filename,functionname):
	f = open(filename)
        g.write("/* Function "+functionname+"*/\n\n")
	line = f.readline()
	while line:
                if len(line) >= 5 + len(functionname): 
                   if line[0:5+len(functionname)] == "int "+functionname+"(":
                        print 'Extracting function', functionname
			while line:
				g.write(line)
                                if line[0] == "}":
	 	                	 break
 		                line = f.readline()
 		        line = f.readline()
                        continue
                if len(line) >= 8 + len(functionname): 
                   if line[0:8+len(functionname)] == "double "+functionname+"(":
                        print 'Extracting ', functionname
   		        while line:
			        g.write(line)
                                if line[0] == "}":
	 	               	   break
 		                line = f.readline()
 		        line = f.readline()
                        continue
                line = f.readline()
	f.close()

def getfunctionF(filename,functionname):
        functionname = lower(functionname)
	newfile = filename + ".f"
	f = open(filename)
	g = open(newfile,"w")
	line = f.readline()
        line = lower(line)
	while line:
                sline = lstrip(line)
                if sline:
                  if len(sline) >= 11 + len(functionname): 
                     if sline[0:11+len(functionname)] == "subroutine "+functionname:
			while line:
                                sline = lstrip(line)
                                if sline:
				  g.write(line)
                                  if sline[0:4] == "end\n":
	 	                     	 break
 		                line = f.readline()
                                line = lower(line)
		line = f.readline()
                line = lower(line)
	f.close()
        g.close()

def main():
    from parseargs import *

    arg_len = len(argv)
    if arg_len < 2: 
        print 'Error! Insufficient arguments.'
        print 'Usage:', argv[0], 'file.[cf] functionname1 functionname2 ...' 
        sys.exit()

    ext = split(argv[1],'.')[-1]
    if ext == "c":
      g = setupfunctionC(argv[1])
      for i in range(2,arg_len):
        getfunctionC(g,argv[1],argv[i])
      g.close()
    else:
      getfunctionF(argv[1],argv[2])

#
# The classes in this file can also be used in other python-programs by using 'import'
#
if __name__ ==  '__main__': 
    main()

