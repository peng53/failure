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

function originLines(xscale, xview, yscale, yview, can, color){
	let ctx = can.getContext('2d');
	ctx.setLineDash([]);
	ctx.strokeStyle = color;
	// line x = 0
	let x0 = transformX(0, xscale, xview[0]);
	drawALine(ctx,x0,x0,0,can.height);
	// line y = 0
	let y0 = transformY(0, yscale, yview[1]);
	drawALine(ctx,0,can.width,y0,y0);
}

function drawALine(ctx, x0, x, y0, y){
	ctx.beginPath();
	ctx.moveTo(x0,y0);
	ctx.lineTo(x,y);
	ctx.stroke();
}

function gridLines(xscale, xview, xinterval, yscale, yview, yinterval, can, hcolor, vcolor){
	let ctx = can.getContext('2d');

	ctx.setLineDash([3,2]);
	ctx.strokeStyle = hcolor;
	let y0 = transformY(yinterval * (Math.floor(yview[1]/yinterval)), yscale, yview[1]);
	let dy = yinterval*yscale;
	for (let y=y0; y<can.height; y+=dy){
		drawALine(ctx, 0, can.width, y, y);
	}

	ctx.setLineDash([2,3]);
	ctx.strokeStyle = vcolor;
	let dx = xinterval*xscale;
	let x0 = transformX(xinterval * (Math.ceil(xview[0]/xinterval)), xscale, xview[0])
	for (let x=x0; x<can.width; x+= dx){
		drawALine(ctx, x, x, 0, can.height);
	}
}

function tickNumbers(xscale, xview, xinterval, yscale, yview, yinterval, can){
	let ctx = can.getContext('2d');
	let y0 = transformY(0, yscale, yview[1]);
	let x0 = transformX(0, xscale, xview[0]);
	let fontSize = 12; // in pixels!
	ctx.font =  fontSize+'px Arial';
	ctx.fillStyle = 'red';
	ctx.textAlign = 'center';
	if (yinterval > 0){
		for (let y=closestMultiple(yview[0], yinterval); y <= yview[1]; y += yinterval){
			ctx.fillText(y, x0, 6+ transformY(y, yscale, yview[1]));
		}
	}
	y0 += fontSize+2; // lower it so its under the originline
	if (xinterval > 0){
		for (let x=closestMultiple(xview[0], xinterval); x <= xview[1]; x += xinterval){
			ctx.fillText(x, transformX(x, xscale, xview[0]), y0);
		}
	}
}

function closestMultiple(n, m){
	return ((n%m)>m/2) ? n+m - n%m : n-n%m;
}

function plotPoints(points, xscale, yscale, xview, yview, can, color, noLine){
	let ctx = can.getContext('2d');
	ctx.beginPath();
	ctx.strokeStyle = ctx.fillStyle = color;
	let x,y;
	for (let i=0, l=points.length; i<l; ++i){
		x=transformX(points[i].x, xscale, xview[0]);
		y=transformY(points[i].y, yscale, yview[1]);
		if (noLine){
			ctx.moveTo(x,y);
		}
		circlePtAt(ctx,x,y);
		if (noLine){
			ctx.fill();
		}
	}
	if (!noLine){
		ctx.setLineDash([]);
		ctx.stroke();
	}
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
	let ctx = can.getContext('2d');
	ctx.fillStyle = color;
	ctx.fillRect(0,0,can.width, can.height);

}

function sortedPointArray(xvals, yvals){
	let points = Array();
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

function SumAccumulator(){
	this.total = 0;
	this.count = 0;
	this.add = function(n){
		this.total += n;
		++this.count;
	};
}

function linearLeastSquares(points){
	// Returns slope and y-intercept from linear least squares on points.
	// Using equation from https://www.mathsisfun.com/data/least-squares-regression.html
	let N = points.length;
	let sumXY = new SumAccumulator();
	points.forEach(p => sumXY.add(p.x*p.y));
	let sumX = new SumAccumulator();
	points.forEach(p => sumX.add(p.x));
	let sumY = new SumAccumulator();
	points.forEach(p => sumY.add(p.y));
	let sumX2 = new SumAccumulator();
	points.forEach(p => sumX2.add(p.x*p.x));

	let m = (((N*sumXY.total) - (sumX.total*sumY.total)) / ((N*sumX2.total) - (sumX.total*sumX.total)));
	let b = (sumY.total - (m*sumX.total)) / N;
	return [m,b];
}
