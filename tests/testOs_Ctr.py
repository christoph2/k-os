
import Os_Ctr as mod
import unittest

mod.OsCtr_InitCounters();

SYS_COUNTER = 0

class TestCounter(unittest.TestCase):

	def setUp(self):
		mod.InitCounter(SYS_COUNTER,0)
		
	def testIncrementCounterOK(self):
		res = mod.IncrementCounter(SYS_COUNTER)
		self.assertEquals(res, mod.E_OK)

	def testIncrementCounterNotOK(self):
		res = mod.IncrementCounter(99)
		self.assertEquals(res, mod.E_OS_ID)

	

unittest.main()
