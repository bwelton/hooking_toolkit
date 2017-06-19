import os
import sys
from PyInstriment import *

if len(sys.argv) < 3:
    print "Usage: python Dump_Synbols.py <binary> <module name>"
    exit(-1)

inst = Instrimenter()
print "Opening binary: " + sys.argv[1]
if inst.OpenBinary(sys.argv[1]) != 0:
    print "Could not open binary " + sys.argv[1]
    exit()

l = inst.GetModuleSymbols(sys.argv[2])
print "Symbols in module " + sys.argv[2]
for x in l:
	print x
