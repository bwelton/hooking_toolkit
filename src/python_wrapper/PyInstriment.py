from ctypes import *

lib = cdll.LoadLibrary("libinst_lib.so")
openBinary = lib.OpenBinary
replaceFunction =  lib.ReplaceFunction
wrapFunction = lib.WrapFunction
performRewrite = lib.PerformRewrite
openBinary.restype = c_void_p

class Instrimenter(object):
	def __init__(self):
		self._storage = None

	def OpenBinary(self, appName):
		self._storage = openBinary(c_char_p(appName))
		if self._storage == None:
			print "Could not open the application binary: " + appName

	def ReplaceFunction(self, functionToReplace, replacementFunction, libraryWithReplacement):
		ret = replaceFunction(self._storage, c_char_p(functionToReplace),
								c_char_p(replacementFunction), c_char_p(libraryWithReplacement))
		if (ret < 0):
			print "We could not setup the replacement for function " + functionToReplace
		return ret

	def WrapFunction(self, functionToWrap, wrapperFunction, libraryWithWrapper, wrapperHookName):
		ret = wrapFunction(self._storage, c_char_p(functionToWrap), c_char_p(wrapperFunction),
							c_char_p(libraryWithWrapper), c_char_p(wrapperHookName))
		if (ret < 0):
			print "We could not setup the wrapper for function " + functionToWrap

	def PerformRewrite(self, outputName):
		ret = performRewrite(self._storage, c_char_p(outputName))
		if (ret < 0):
			print "Binary rewrite has failed"
