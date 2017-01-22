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
			self.min_x = min(self.min_x,x)
			self.max_x = max(self.max_x,x)
			self.min_y = min(self.min_y,y)
			self.max_y = max(self.max_y,y)
		print self.min_x,self.max_x
		print self.min_y,self.max_y
		Tk.Button(master, text="Close", command=quit).pack()
		self.w = Tk.Canvas(master, width=(abs(self.min_x)+self.max_x)+1, height=(abs(self.min_y)+self.max_y)+1, background="white")
		self.w.pack()
		self.w.create_line(0,self.max_y,abs(self.min_x)+self.max_x,self.max_y,fill="grey")
		self.w.create_line(abs(self.min_x),0,abs(self.min_x),abs(self.min_y)+self.max_y,fill="grey")
		for (x,y) in pts:
			cy = -y+self.max_y
			cx = x+abs(self.min_x)
			self.w.create_oval(cx,cy,cx+1,cy+1,width=0.5)

def draw_graph(pts):
	root = Tk.Tk()
	app = XYPlane(root,pts)
	root.mainloop()

if __name__=="__main__":
	pts = [(x,x**2) for x in xrange(-20,20,1)]+[(-2,-4),(10,-20)]+[(x,x) for x in xrange(-100,100,2)]
	draw_graph(pts)

