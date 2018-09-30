Failure REPO

Solutions thought of in the mind or written on paper work so well..
until actually coding it. Those 'small' details end up becoming the
straw that breaks the coder's back.

So far the following details caught me off guard:

	scaling a graph! per pixel mapping is great for large graphs,
		but the beautiful x^2 and x^3 curves MUST be seen on [-1,1].

	positioning nodes on a canvas! a grid formation was what I had
		in mind, but this prevents many lines from being visible;
		randomly shifting the psoitions doesn't work either. the sol-
		ution was arranging the nodes on a circle; no wonder clocks
		are round.

	spliting a list/array to about equal parts..  no comment, one off
		errors are annoying

	user interface. so the program does x, but is the interface a
		chore to use? what I am actually allowed to do?

	repeated code. should I move this section to a dedicated file?
		is it important enough? case in point: draw_biclique,
		draw_graph, and graph. (and draw_xy)

I'm just trying to make everything work as I intended and then some.

Also, this repo is just a hobby.


// TESTING COMMIT
