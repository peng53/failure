
class InProgress:
	def __init__(self):
		pass

	def get(self):
		pass

class BinaryStateIP(InProgress):
	def __init__(self, states: str = ['/\r','\\\r']):
		self.state = False
		self.states = states

	def out(self):
		if self.state is True:
			print(self.states[int(self.state)], end='')
		else:
			print(self.states[int(self.state)], end='')
		self.state = not self.state

class NStateIP(BinaryStateIP):
	def __init__(self, states: str = ['/\r','\\\r']):
		self.state = 0
		self.states = states
		self.mstate = len(states)
	
	def out(self):
		print(self.states[self.state], end='')
		self.state += 1
		if self.state == self.mstate:
			self.state = 0

from time import sleep

SpinningLine = NStateIP(['-\r', '\\\r', '|\r', '/\r'])
Pac = NStateIP(['<ooooo\r', ' <oooo\r', '  <ooo\r', '   <oo\r', '    <o\r', '     <\r'])
Xss = NStateIP(['x+x\r', '+x+\r'])
for i in range(35):
	Xss.out()
	sleep(1)
