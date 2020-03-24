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

function originLines(xscale, xview, yscale, yview, can){
	let ctx = can.getContext('2d');
	ctx.setLineDash([]);
	ctx.strokeStyle = 'red';
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

function gridLines(xscale, xview, xinterval, yscale, yview, yinterval, can){
	let ctx = can.getContext('2d');

	ctx.setLineDash([3,2]);
	ctx.strokeStyle = 'lime';
	let y0 = transformY(yinterval * (Math.floor(yview[1]/yinterval)), yscale, yview[1]);
	let dy = yinterval*yscale;
	for (let y=y0; y<can.height; y+=dy){
		drawALine(ctx, 0, can.width, y, y);
	}

	ctx.setLineDash([2,3]);
	ctx.strokeStyle = 'teal';
	let dx = xinterval*xscale;
	for (let x=Math.ceil(xview[0]/xinterval)*xscale; x<can.width; x+= dx){
		drawALine(ctx, x, x, 0, can.height);
	}
}

function plotPoints(points, xscale, yscale, xview, yview, can, color){
	let ctx = can.getContext('2d');
	ctx.beginPath();
	for (let i=0, l=points.length; i<l; ++i){
		circlePtAt(
			ctx,
			x=transformX(points[i].x, xscale, xview[0]),
			y=transformY(points[i].y, yscale, yview[1]));
	}
	ctx.setLineDash([]);
	ctx.strokeStyle = color;
	ctx.stroke();
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
		m = Math.min(m, n, N);
		M = Math.max(M, n, N);
	}
	return [m, M];
}

function canvasBg(can){
	let ctx = can.getContext('2d');
	ctx.fillStyle = 'white';
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
function Dataset(name, color){
	this.name = name;
	this.color = color;
	this.points = [];
}
