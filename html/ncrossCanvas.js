import { NCrossBoard } from './ncross.js';

export class NCrossCanvas {
	constructor(canvas,r,c,states){
		this.canvas = canvas;
		this.board = new NCrossBoard(r,c,states.length);
		this.metrics = {
			hintReserve : 100
		,	twidth : (canvas.width-100)/c
		,	theight : (canvas.height-100)/r
		,	fontlb : 10
		};
		this.canvas.addEventListener('click',(e) => {
			const {c,r} = this.xyToRC(event.offsetX,event.offsetY) || 0;
			if (c!=undefined){
				this.clickedRecalc(r,c);
			}
		});
		this.colorStates = states;
	}
	generate(){
		this.board.generate();
		this.draw();
	}
	draw(){
		const ctx = this.canvas.getContext('2d');
		ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
		for (let r=0;r<this.board.height;++r){
			for (let c=0;c<this.board.width;++c){
				if (this.board.A[r][c]!=0){
					this.drawTile(ctx, r, c);
				}
			}
		}
		ctx.font = `${this.metrics.fontlb}px Arial`;
		for (let r=0;r<this.board.height;++r){
			this.drawRHint(ctx,r);
		}
		for (let c=0;c<this.board.width;++c){
			this.drawCHint(ctx, c);
		}
		this.drawLines(ctx);
	}
	drawTile(ctx,r,c){
		ctx.fillStyle = this.colorStates[this.board.A[r][c]];
		ctx.fillRect(
			c*this.metrics.twidth+this.metrics.hintReserve
		,	r*this.metrics.theight+this.metrics.hintReserve
		,	this.metrics.twidth
		,	this.metrics.theight
		);
	}
	drawRHint(ctx,r){
		ctx.textAlign = 'left';
		ctx.textBaseline = 'middle';
		for (let i=0;i<this.board.rowHints[r].length;++i){
			ctx.strokeStyle = this.colorStates[this.board.rowHints[r][i].state];
			ctx.strokeText(
				this.board.rowHints[r][i].cnt
			,	i*this.metrics.fontlb
			,	r*this.metrics.theight+this.metrics.theight/2+this.metrics.hintReserve
			);
		}
	}
	drawCHint(ctx,c){
		ctx.textAlign = 'center';
		ctx.textBaseline = 'top';
		for (let i=0;i<this.board.colHints[c].length;++i){
			ctx.strokeStyle = this.colorStates[this.board.colHints[c][i].state];
			ctx.strokeText(
				this.board.colHints[c][i].cnt
			,	c*this.metrics.twidth+this.metrics.hintReserve+this.metrics.twidth/2
			,	i*this.metrics.fontlb
			);
		}
	}
	drawLines(ctx){
		ctx.strokeStyle = 'black';
		ctx.beginPath();
		for (let r=0;r<this.board.height;++r){
			ctx.moveTo(0,this.metrics.hintReserve+r*this.metrics.theight);
			ctx.lineTo(this.canvas.width,this.metrics.hintReserve+r*this.metrics.theight);
		}
		for (let c=0;c<this.board.width;++c){
			ctx.moveTo(this.metrics.hintReserve+c*this.metrics.twidth, 0);
			ctx.lineTo(this.metrics.hintReserve+c*this.metrics.twidth, this.canvas.height);
		}
		ctx.stroke();
		ctx.closePath();
	}
	clickedRecalc(r,c){
		// need to toggle state of tile.
		this.board.A[r][c] = (this.board.A[r][c]+1)%this.board.states;
		this.board.recalcCol(c);
		this.board.recalcRow(r);
		const ctx = this.canvas.getContext('2d');
		ctx.font = `${this.metrics.fontlb}px Arial`;
		this.clearRHint(ctx,r);
		this.drawRHint(ctx,r);
		this.clearCHint(ctx,c);
		this.drawCHint(ctx,c);
		if (this.board.A[r][c]!=0){
			this.drawTile(ctx,r,c);
		} else {
			this.clearTile(ctx,r,c);
		}
	}
	clearTile(ctx, r,c){
		ctx.clearRect(
			c*this.metrics.twidth+this.metrics.hintReserve
		,	r*this.metrics.theight+this.metrics.hintReserve
		,	this.metrics.twidth
		,	this.metrics.theight
		);
	}
	clearRHint(ctx,r){
		ctx.clearRect(
			0
		,	this.metrics.hintReserve+r*this.metrics.twidth
		,	this.metrics.hintReserve
		,	this.metrics.twidth
		);
	}
	clearCHint(ctx,c){
		ctx.clearRect(
			this.metrics.hintReserve+c*this.metrics.twidth
		,	0
		,	this.metrics.theight
		,	this.metrics.hintReserve
		);
	}
	xyToRC(x,y){
		x -= this.metrics.hintReserve;
		y -= this.metrics.hintReserve;
		if (x>=0 && y>=0) return {c : Math.floor(x/this.metrics.twidth), r : Math.floor(y/this.metrics.theight)};
	}
	recalMetrics(){
		this.metrics.twidth = (this.canvas.width-this.metrics.hintReserve)/this.board.width;
		this.metrics.theight = (this.canvas.height-this.metrics.hintReserve)/this.board.height;
		console.log(this.metrics);
		this.draw();
	}
	set hintReserve(p){
		if (p!=this.metrics.hintReserve){
			this.metrics.hintReserve = p;
			this.recalMetrics();
		}
	}
	resizeBoard(r,c){
		if (r!=this.board.height || c!=this.board.width){
			this.board.width = c;
			this.board.height = r;
			this.board.clear();
			console.log(r,c);
		}
		this.board.generate();
		this.recalMetrics();
	}
}
