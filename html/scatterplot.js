function transformX(x, scale, xOffset){
	// Transforms to pixel coord
	return scale * (x-xOffset);
}
function transformY(y, scale, yOffset){
	// Transforms to pixel coord
	return scale * (yOffset-y);
}
function calculateScale(view, outputLength){
	return outputLength / Math.abs(Math.max(view[1] - view[0], 1));
}

function circlePtAt(ctx,x,y){
	ctx.arc(x,y,1,0,2* Math.PI);
}
function dotPtAt(ctx,x,y){
	ctx.fillRect(x-1,y-1,2,2);
}

function drawXAxis(can, xview, dx, offset){
	const xscale = calculateScale(xview, can.width);
	const ctx = can.getContext('2d');
	//ctx.strokeStyle = 'gray';
	drawALine(ctx,0,can.width,0,0);
	const dZ = xscale*dx;
	ctx.beginPath();
	for (let x=closestMultiple(xview[0],dx), rx=0; x <=xview[1]; x+=dx){
		ctx.strokeStyle = (x>=0) ? 'black' : 'red';
		ctx.strokeText(Math.abs(x), rx, offset);
		ctx.moveTo(rx, -2);
		ctx.lineTo(rx, 2);
		rx += dZ;
	}
	ctx.strokeStyle = 'black';
	ctx.stroke();
}
function drawYAxis(can, yview, dy, offset, noshow){
	const ctx = can.getContext('2d');
	const yscale = calculateScale(yview, can.height);
	//ctx.strokeStyle = 'gray';
	drawALine(ctx,0,0,0,can.height);
	ctx.rotate(-90* Math.PI/180);
	const dZ = yscale*dy;
	ctx.beginPath();
	for (let y=closestMultiple(yview[0],dy), ry=can.height; y<=yview[1]; y+=dy){
		if (y != noshow){
			ctx.strokeStyle = (y>=0) ? 'black' : 'red';
			ctx.strokeText(Math.abs(y),-ry,-offset);
		}
		ctx.moveTo(-ry, -2);
		ctx.lineTo(-ry, 2);
		ry -= dZ;
		
	}
	ctx.strokeStyle = 'black';
	ctx.stroke();
}
function axisLines(xview, yview, intercepts, intervals, hcolor, vcolor, fontsize){
	const can = document.getElementById('axisl');
	const ctx = can.getContext('2d');
	ctx.clearRect(0, 0, can.width, can.height);
	ctx.textBaseline = 'middle';
	ctx.textAlign = 'center';
	ctx.font = fontsize.toString()+'pt Courier';

	ctx.save();
	ctx.translate(0, calculateScale(yview, can.height)*(yview[1]-intercepts[1]));
	ctx.strokeStyle = hcolor;
	drawXAxis(can,xview,intervals[0], fontsize);
	ctx.restore();

	ctx.save();
	ctx.translate(calculateScale(xview, can.width)*(intercepts[0]-xview[0]), 0);
	ctx.strokeStyle = vcolor;
	drawYAxis(can,yview,intervals[1], fontsize, noshow=intercepts[1]);
	ctx.restore();
}

function drawALine(ctx, x0, x, y0, y){
	ctx.beginPath();
	ctx.moveTo(x0,y0);
	ctx.lineTo(x,y);
	ctx.stroke();
}

function gridLines(xscale, xview, xinterval, yscale, yview, yinterval, can, hcolor, vcolor){
	const ctx = can.getContext('2d');

	ctx.setLineDash([3,2]);
	ctx.strokeStyle = hcolor;
	const y0 = transformY(yinterval * (Math.floor(yview[1]/yinterval)), yscale, yview[1]);
	const dy = yinterval*yscale;
	for (let y=y0; y<can.height; y+=dy){
		drawALine(ctx, 0, can.width, y, y);
	}

	ctx.setLineDash([2,3]);
	ctx.strokeStyle = vcolor;
	const dx = xinterval*xscale;
	const x0 = transformX(xinterval * (Math.ceil(xview[0]/xinterval)), xscale, xview[0])
	for (let x=x0; x<can.width; x+= dx){
		drawALine(ctx, x, x, 0, can.height);
	}
}

function closestMultiple(n, m){
	return ((n%m)>m/2) ? n+m - n%m : n-n%m;
}

function plotPoints(points, xscale, yscale, xview, yview, can, color, noLine){
	const ctx = can.getContext('2d');
	ctx.beginPath();
	ctx.strokeStyle = ctx.fillStyle = color;
	let x,y;
	let firstPt = true;
	if (!noLine){
		ctx.beginPath();
	}
	for (let i=0, l=points.length; i<l; ++i){
		x=transformX(points[i].x, xscale, xview[0]);
		y=transformY(points[i].y, yscale, yview[1]);
		if (!noLine){
			if (firstPt){
				ctx.moveTo(x,y);
				firstPt = false;
			} else {
				ctx.lineTo(x,y);
			}
		}
		dotPtAt(ctx,x,y);
	}
	if (!noLine){
		ctx.setLineDash([]);
		ctx.stroke();
	}
	// linearLeastSquares test!
	const [m,b] = linearLeastSquares(points);
	ctx.globalAlpha = 0.4;
	ctx.strokeStyle = '#FF0000';
	ctx.setLineDash([4,4]);
	drawALine(
		ctx,
		transformX(xview[0], xscale, xview[0]),
		transformX(xview[1], xscale, xview[0]),
		transformY(m*xview[0]+b, yscale, yview[1]),
		transformY(m*xview[1]+b, yscale, yview[1])
	);
	ctx.globalAlpha = 1;
}

function getMinAndMax(values, getter){
	let m = getter(values[0]);
	let M = m;
	let v;
	for (let i=1; i<values.length; ++i){
		v = getter(values[i]);
		if (v<m){
			m = v;
		} else if (v>M){
			M = v;
		}
	}
	return [m, M];
}

function getMinAndMaxOverAll(datasets, getter){
	let [m, M] = getMinAndMax(datasets[0].points, getter);
	let n, N;
	for (let i=1; i<datasets.length; ++i){
		[n, N] = getMinAndMax(datasets[i].points, getter);
		m = Math.min(m, n);
		M = Math.max(M, N);
	}
	return [m, M];
}

function canvasBg(can, color){
	const ctx = can.getContext('2d');
	ctx.fillStyle = color;
	ctx.fillRect(0,0,can.width, can.height);
}

function sortedPointArray(xvals, yvals){
	const points = Array();
	for (let i=0, l=xvals.length; i<l; ++i){
		points.push(new Point(xvals[i],yvals[i]));
	}
	points.sort((a,b) => a.x-b.x);
	return points;
}

function Point(x,y){
	this.x = x;
	this.y = y;
}
function Graph(){
	this.datasets = [];
}
function Dataset(name, color, connectPoints){
	this.name = name;
	this.color = color;
	this.points = [];
	this.connectPoints = connectPoints
}

function linearLeastSquares(points){
	// Returns slope and y-intercept from linear least squares on points.
	// Using equation from https://www.mathsisfun.com/data/least-squares-regression.html
	const N = points.length;
	const sumXY = points.reduce((total,p) => total + p.x*p.y, 0);
	const sumX = points.reduce((total,p) => total + p.x, 0);
	const sumY = points.reduce((total,p) => total + p.y, 0);
	const sumX2 = points.reduce((total,p) => total + p.x*p.x, 0);
	
	const m = (((N*sumXY) - (sumX*sumY)) / ((N*sumX2) - (sumX*sumX)));
	const b = (sumY - (m*sumX)) / N;
	return [m,b];
}
