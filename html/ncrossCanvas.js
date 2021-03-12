import { NCrossBoard } from './ncross.js';

export class NCrossCanvas {
	constructor(canvas,r,c){
		this.canvas = canvas;
		this.board = new NCrossBoard(r,c);
		this.metrics = {
			hintReserve : 100
		,	twidth : (canvas.width-100)/c
		,	theight : (canvas.height-100)/r
		,	fontlb : 10
		}
	}
	generate(){
		this.board.generate();
		this.draw();
	}
	draw(){
		const ctx = this.canvas.getContext('2d');
		ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
		ctx.fillStyle = 'red';
		for (let r=0;r<this.board.height;++r){
			for (let c=0;c<this.board.width;++c){
				if (this.board.A[r][c]==1){
					this.drawTile(ctx, r, c);
				}
			}
		}
		for (let r=0;r<this.board.height;++r){
			this.drawRHint(ctx,r);
		}
		for (let c=0;c<this.board.width;++c){
			this.drawCHint(ctx, c);
		}
		this.drawLines(ctx);
	}
	drawTile(ctx,r,c){
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
			ctx.strokeText(
				this.board.rowHints[r][i]
			,	i*this.metrics.fontlb
			,	r*this.metrics.theight+this.metrics.theight/2+this.metrics.hintReserve
			);
		}
	}
	drawCHint(ctx,c){
		ctx.textAlign = 'center';
		ctx.textBaseline = 'top';
		for (let i=0;i<this.board.colHints[c].length;++i){
			ctx.strokeText(
				this.board.colHints[c][i]
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
}
