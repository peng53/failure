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
		self.scale = 500
		self.w = Tk.Canvas(master, width=self.scale*(abs(self.min_x)+self.max_x)+1, height=self.scale*(abs(self.min_y)+self.max_y)+1, background="white")
		self.w.pack()
		self.w.create_line(0,self.scale*self.max_y,self.scale*(abs(self.min_x)+self.max_x),self.scale*self.max_y,fill="grey")
		self.w.create_line(self.scale*abs(self.min_x),0,self.scale*abs(self.min_x),self.scale*(abs(self.min_y)+self.max_y),fill="grey")
		for (x,y) in pts:
			cy = self.scale*(-y+self.max_y)
			cx = self.scale*(x+abs(self.min_x))
			self.w.create_oval(cx,cy,cx+1,cy+1,width=0.5)

def frange(a,b,d):
	x = a
	while x<b:
		yield x
		x += d

def draw_graph(pts):
	root = Tk.Tk()
	app = XYPlane(root,pts)
	root.mainloop()



if __name__=="__main__":
	pts = [(x,x*x) for x in frange(-1.0,1.0,0.1)]+[(x,x) for x in frange(-1.0,1.0,0.1)]
	print pts
	draw_graph(pts)

