import os
import sys
from PyInstriment import *

if len(sys.argv) < 3:
    print "Usage: python Instriment_Binary.py <binary> <replacement_description_file>"
    exit(-1)

inst = Instrimenter()
print "Opening binary: " + sys.argv[1]
if inst.open(sys.argv[1]) != 0:
    print "Could not open binary " + sys.argv[1]
    exit()
    



