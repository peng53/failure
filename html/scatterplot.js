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

function axisLines(xscale, xview, yscale, yview, can, color, xy, xyn){
	let ctx = can.getContext('2d');
	ctx.setLineDash([]);
	ctx.strokeStyle = color;
	console.log(xy);
	let x0, y0;
	// line x = 0
	if (xy[0] >= xview[0] && xy[0] <= xview[1]){
		x0 = transformX(xy[0], xscale, xview[0]);
		drawALine(ctx,x0,x0,0,can.height);
	}
	// line y = 0
	if (xy[1] >= yview[0] && xy[1] <= yview[1]){
		y0 = transformY(xy[1], yscale, yview[1]);
		drawALine(ctx,0,can.width,y0,y0);
	}
	if (x0 && y0 && xyn && !xyn.some(isNaN)){
		// both axis are visible so draw numbers..
		let fontSize = 12; // in pixels!
		ctx.font =  fontSize+'px monospace';
		ctx.textAlign = 'right';
		ctx.fillStyle = 'orange';
		ctx.textBaseline = 'middle';
		//x0 -= 2;
		//ctx.rotate(90 * Math.PI/180);
		let y=closestMultiple(yview[0], xyn[1]);
		let x=closestMultiple(xview[0], xyn[0]);
		let dx = xscale * xyn[0];
		let dy = yscale * xyn[1];
		for (let sy = transformY(y, yscale, yview[1]); y <= yview[1]; y += xyn[1]){
			ctx.fillText(y, x0, sy);
			sy -= dy;
		}
		//ctx.rotate(-90 * Math.PI/180);
		ctx.textAlign = 'center';
		ctx.fillStyle = 'teal';
		ctx.textBaseline = 'hanging';
		//y0 += fontSize/2;
		//0 += 2;
		for (let sx = transformX(x, xscale, xview[0]); x <= xview[1]; x += xyn[0]){
			ctx.fillText(x, sx, y0);
			sx += dx;
		}
	}
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
	let [m,b] = linearLeastSquares(points);
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

function linearLeastSquares(points){
	// Returns slope and y-intercept from linear least squares on points.
	// Using equation from https://www.mathsisfun.com/data/least-squares-regression.html
	let N = points.length;
	let sumXY = points.reduce((total,p) => total + p.x*p.y, 0);
	let sumX = points.reduce((total,p) => total + p.x, 0);
	let sumY = points.reduce((total,p) => total + p.y, 0);
	let sumX2 = points.reduce((total,p) => total + p.x*p.x, 0);
	
	let m = (((N*sumXY) - (sumX*sumY)) / ((N*sumX2) - (sumX*sumX)));
	let b = (sumY - (m*sumX)) / N;
	return [m,b];
}
