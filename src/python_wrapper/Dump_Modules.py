import os
import sys
from PyInstriment import *

if len(sys.argv) < 2:
    print "Usage: python Dump_Modules.py <binary>"
    exit(-1)

inst = Instrimenter()
print "Opening binary: " + sys.argv[1]
if inst.OpenBinary(sys.argv[1]) != 0:
    print "Could not open binary " + sys.argv[1]
    exit()

l = inst.GetModules()
print "Modules in this binary:"
for x in l:
	print x
