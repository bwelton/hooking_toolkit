import os
import sys
from PyInstriment import *

if len(sys.argv) < 3:
    print "Usage: python Find_Symbol_Prefix.py <binary> <prefix>"
    exit(-1)

inst = Instrimenter()
print "Opening binary: " + sys.argv[1]
if inst.OpenBinary(sys.argv[1]) != 0:
    print "Could not open binary " + sys.argv[1]
    exit()

l = inst.FindSymbolPrefix(sys.argv[2])
print "Symbols matching prefix " + sys.argv[2]
for x in l:
	print x
