
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
		

from time import sleep

d = BinaryStateIP()
for i in range(35):
	d.out()
	sleep(1)
