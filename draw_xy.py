"""
Phase 1: draw points where x&y>=0
Phase 2: allow negative x&y
Phase 3: connect points like a line graph
.
.
.
"""
import Tkinter as Tk

class XYPlane:
	def __init__(self,master,pts):
		self.pts = pts
		self.min_x, self.max_x = 0, 0
		self.min_y, self.max_y = 0, 0
		for (x,y) in pts:
			self.max_x = max(self.max_x,x)
			self.max_y = max(self.max_y,y)
		self.w = Tk.Canvas(master, width=(abs(self.min_x)+self.max_x)+10, height=(abs(self.min_y)+self.max_y)+10, background="white")
		self.w.pack()
		for (x,y) in pts:
			self.w.create_oval(x,y,x+1,y+1)

def draw_graph(pts):
	root = Tk.Tk()
	app = XYPlane(root,pts)
	root.mainloop()

if __name__=="__main__":
	pts = [(x,x**2) for x in xrange(50)]
	draw_graph(pts)

